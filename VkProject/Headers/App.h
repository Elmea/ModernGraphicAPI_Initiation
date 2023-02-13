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
	const std::vector<const char*> validationLayers { "VK_LAYER_KHRONOS_validation" };

	const std::vector<const char*> deviceExtensions { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
														VkDebugUtilsMessageTypeFlagsEXT messageType,
														const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
														void* pUserData);

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool IsComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails 
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};


	class App
	{
	public:
		App(WindowInfo windowInfo);
		~App() { Destroy(); }
		
		void Run();

	private:
		const int MAX_FRAMES_IN_FLIGHT = 2;

		uint32_t currentFrame = 0;

		Window* m_window;

		VkInstance m_instance;

		uint32_t m_glfwExtensionCount = 0;
		const char** m_glfwExtensions;
		VkDebugUtilsMessengerEXT m_debugMessenger;

		void MainLoop();
		void DrawFrame();
		void Destroy();

		#pragma region VkSetup
		// Physical device var
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		QueueFamilyIndices physicalDeviceQueueFamily;

		// Logical device var
		VkDevice m_logicalDevice;
		VkQueue m_graphicsQueue;

		// Window var
		VkSurfaceKHR m_surface;
		VkQueue m_presentQueue;

		// SwapChain var
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;

		std::vector<VkFramebuffer> swapChainFramebuffers;

		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

		// Pipeline var
		VkPipeline graphicsPipeline;
		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;

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
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		bool IsDeviceSuitable(VkPhysicalDevice device);
		void PickPhysicalDevice();

		// Vulkan logical device Setup
		void CreateLogicalDevice();

		// Vulkan window Setup
		void CreateSurface();
		
		// Vulkan Swapchain setup
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void CreateSwapChain();
		void CreateFramebuffers();

		// Image view setup
		void CreateImageViews();

		// Pipeline
		void CreateGraphicsPipeline();
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		void CreateRenderPass();

		void CreateCommandPool();
		void CreateCommandBuffer();
		void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		void CreateSyncObjects();

		#pragma endregion
	};
}