#pragma once


#include "Window.h"
#include "VkRenderer.h"
#include "Camera.h"

namespace Core
{
	struct Inputs
	{
		bool forward, backward;
		bool left, right;
		bool up, down;

		double mouseX = 0.0;
		double mouseY = 0.0;
		float mouseDeltaX = 0.0;
		float mouseDeltaY = 0.0;
	};

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

		bool mouseCaptured = false;

		float rotationSpeed = 45.0f;
		Inputs inputs;

		Camera* camera;

		void MainLoop();
		void ProcessInput();
	};
}