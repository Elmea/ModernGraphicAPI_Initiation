#pragma once
#include <vulkan/vulkan.h>
#include <optional>
#include <vector>

#include "Window.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
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

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;

		bool IsComplete()
		{
			return graphicsFamily.has_value();
		}
	};

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

		// Physical device var
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		QueueFamilyIndices physicalDeviceQueueFamily;

		// Logical device var
		VkDevice m_logicalDevice;
		VkQueue m_graphicsQueue;


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

		// Vulkan logical device Setup
		void CreateLogicalDevice();
	};
}