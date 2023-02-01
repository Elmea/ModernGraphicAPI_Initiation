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
		void Awake(WindowInfo windowInfo);
		void Run();
		void Destroy();

	private:
		VkInstance m_instance;
		Window* m_window;

		uint32_t m_glfwExtensionCount = 0;
		const char** m_glfwExtensions;
		VkDebugUtilsMessengerEXT m_debugMessenger;

		void MainLoop();
		void InitVulkan();
		void CreateVkInstance();
		bool CheckValidationLayerSupport();
		std::vector<const char*> GetDebugExtensions();
		void SetupDebugger();
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
		void DebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	};
}