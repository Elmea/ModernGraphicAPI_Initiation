#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "Window.h"

#ifdef NDEBUG
#   define assert(condition) ((void)0)
const bool enableValidationLayers = false;
#else
#   define assert(condition) 
const bool enableValidationLayers = true;
#endif


namespace Core
{
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	class App
	{
	public:
		App(WindowInfo windowInfo);
		~App() { Destroy(); }
		
		void Run();

	private:
		Window* m_window;

		VkInstance m_instance;

		uint32_t m_glfwExtensionCount = 0;
		const char** m_glfwExtensions;
		VkDebugUtilsMessengerEXT m_debugMessenger;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		void MainLoop();
		void Destroy();
		
		// Vulkan Initialisation methods
		void InitVulkan();
		void CreateVkInstance();
		bool CheckValidationLayerSupport();

		// Vulkan debug initialisation methods
		std::vector<const char*> GetDebugExtensions();
		void SetupDebugger();
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
		void DebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		// Vulkan GPU setup
		void PickPhysicalDevice();
	};
}