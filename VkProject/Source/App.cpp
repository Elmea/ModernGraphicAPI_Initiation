#include "glfwInclude.h"

#include <stdexcept>
#include <iostream>
#include <map>
#include <set>

#include "App.h"

using namespace Core;


App::App(WindowInfo windowInfo) 
{
    glfwInit();
    m_window = new Window(windowInfo);
    m_glfwExtensions = glfwGetRequiredInstanceExtensions(&m_glfwExtensionCount);
}

#pragma region VulkanInit

    #pragma region DebugerSetup
static VKAPI_ATTR VkBool32 VKAPI_CALL Core::DebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
                                                        void* pUserData)
{
    switch (messageSeverity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        std::cout << callbackData->pMessage << std::endl;
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        std::cout << "\033[33;1m" << callbackData->pMessage << "\033[0m\n";
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        std::cout << "\033[31;1m"<< callbackData->pMessage <<"\033[0m\n";
        break;
    default:
        break;
    }

    return VK_FALSE;
}

std::vector<const char*> App::GetDebugExtensions()
{
    std::vector<const char*> extensions(m_glfwExtensions, m_glfwExtensions + m_glfwExtensionCount);

    if (enableValidationLayers) 
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
}

VkResult App::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto functionPtr = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (functionPtr)
    {
        return functionPtr(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else 
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void App::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void App::DebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugCallBack;
}

void App::SetupDebugger()
{
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    DebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}
    #pragma endregion

    #pragma region GPUSetup

bool checkDeviceExtensionSupport(VkPhysicalDevice device) 
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

QueueFamilyIndices App::FindQueueFamilies(VkPhysicalDevice device) 
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (int i = 0; i < queueFamilyCount; i++)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) 
            indices.graphicsFamily = i;

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);
        
        if (presentSupport) 
            indices.presentFamily = i;

        if (indices.IsComplete()) break;
    }

    return indices;
}

int RateDeviceSuitability(VkPhysicalDevice device) 
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // Application can't function without geometry shaders
    if (!deviceFeatures.geometryShader) {
        return 0;
    }

    int score = 0;

    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    score += deviceProperties.limits.maxImageDimension2D;

    return score;
}

bool App::IsDeviceSuitable(VkPhysicalDevice device) 
{
    QueueFamilyIndices indices = FindQueueFamilies(device);

    return indices.IsComplete() && checkDeviceExtensionSupport(device);
}

void App::PickPhysicalDevice()
{
    uint32_t deviceCount = 5;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    
    if (deviceCount == 0) throw std::runtime_error("Can't find GPUs with Vulkan support");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());


    std::multimap<int, VkPhysicalDevice> candidates;

    for (const VkPhysicalDevice device : devices)
    {
        if (!IsDeviceSuitable(device))
            continue;

        int score = RateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    if (candidates.rbegin()->first > 0)
    {
        physicalDevice = candidates.rbegin()->second;
        physicalDeviceQueueFamily = FindQueueFamilies(physicalDevice);

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

        std::cout << "\033[32;1m" << "Physical device selected : \033[36m" << deviceProperties.deviceName << "\033[0m\n";
        return;
    }

    throw std::runtime_error("Failed to find a suitable GPU for vulkan");
}
    #pragma endregion

    #pragma region LogicalDeviceSetup
void App::CreateLogicalDevice()
{
    VkDeviceQueueCreateInfo queueCreateInfo{};

    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = physicalDeviceQueueFamily.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo lDeviceCreateInfo{};
    lDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies{ physicalDeviceQueueFamily.graphicsFamily.value(), physicalDeviceQueueFamily.presentFamily.value() };

    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    lDeviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    lDeviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();


    VkPhysicalDeviceFeatures deviceFeatures{};
    lDeviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    lDeviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    lDeviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers) 
    {
        lDeviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        lDeviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else 
    {
        lDeviceCreateInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice, &lDeviceCreateInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
        throw std::runtime_error("Failed to create a logical device");

    vkGetDeviceQueue(m_logicalDevice, physicalDeviceQueueFamily.presentFamily.value(), 0, &m_graphicsQueue);

    std::cout << "\033[32;1m" << "Logical device created successfuly" << "\033[0m" << std::endl;
}
    #pragma endregion

    #pragma region WindowSetup
void App::CreateSurface()
{
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = glfwGetWin32Window(m_window->Get());
    createInfo.hinstance = GetModuleHandle(nullptr);

    if (glfwCreateWindowSurface(m_instance, m_window->Get(), nullptr, &m_surface) != VK_SUCCESS)
        throw std::runtime_error("Failed to create window surface");
    
    std::cout << "\033[32;1m" << "Window surface created successfuly" << "\033[0m" << std::endl;
}
    #pragma endregion

    #pragma region SwapChainSetup
SwapChainSupportDetails App::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;
    
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

    return details;
}
    #pragma endregion

    #pragma region Setup
void App::CreateVkInstance()
{
    if (enableValidationLayers && !CheckValidationLayerSupport()) 
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo info{};
    info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    info.pApplicationName = "Vk project";
    info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    info.pEngineName = "No engine";
    info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &info;

    createInfo.enabledExtensionCount = m_glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = m_glfwExtensions;

    std::vector<const char*> extenstion = GetDebugExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extenstion.size());
    createInfo.ppEnabledExtensionNames = extenstion.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) 
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();   
        DebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else 
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create instance!");
    }
    else
    {
        std::cout << "\033[32;1mVulkan Instance created succefuly\033[0m" << std::endl;
    }
}

bool App::CheckValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

void App::InitVulkan()
{
    CreateVkInstance();
    SetupDebugger();
    CreateSurface();
    PickPhysicalDevice(); 
    CreateLogicalDevice();
}
    #pragma endregion

#pragma endregion

void App::Run()
{
    try
    {
        InitVulkan();
        MainLoop();
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << std::endl;
        return;
    }
}

void App::MainLoop()
{
    while (!glfwWindowShouldClose(m_window->Get()))
    {
        glfwPollEvents();
    }
}

void App::Destroy() 
{
    if (enableValidationLayers) 
        DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);

    vkDestroyDevice(m_logicalDevice, nullptr);
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    vkDestroyInstance(m_instance, nullptr);

    m_window->Destroy();
    delete m_window;
    glfwTerminate();
}