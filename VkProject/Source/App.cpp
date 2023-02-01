#include <stdexcept>
#include <iostream>
#include <cassert>

#include "App.h"

#ifdef NDEBUG
#  define assert(condition) ((void)0)
#else
#  define assert(condition)
#endif

using namespace Core;

void App::Awake(WindowInfo windowInfo) 
{
    glfwInit();
    m_window = new Window(windowInfo);
    m_glfwExtensions = glfwGetRequiredInstanceExtensions(&m_glfwExtensionCount);
}

void App::CreateVkInstance()
{
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

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create instance!");
    }
    else
    {
        std::cout << "Vulkan Instance created succefuly" << std::endl;
    }
}

void App::InitVulkan()
{
    CreateVkInstance();
}

void App::Run()
{
    InitVulkan();

    try
    {
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
    vkDestroyInstance(m_instance, nullptr);

    m_window->Destroy();
    delete m_window;
    glfwTerminate();
}