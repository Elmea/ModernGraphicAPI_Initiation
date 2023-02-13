#pragma once


#include "Window.h"
#include "VkRenderer.h"

namespace Core
{
	class App
	{
	public:
		App(WindowInfo windowInfo);
		~App();
		
		void SetFramebufferResized(bool boolean) { vkRenderer->framebufferResized = boolean; }

		void Run();

	private:

		VkRenderer* vkRenderer;

		Window* m_window;

		void MainLoop();
	};
}