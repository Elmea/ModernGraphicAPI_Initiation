#pragma once
#include <vulkan/vulkan.h>

#include "Window.h"

namespace Core
{
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

		void MainLoop();
		void InitVulkan();
		void CreateVkInstance();
	};
}