#pragma once
#include <string>

#include "glfwInclude.h"

namespace Core
{
	class App;

	struct WindowInfo
	{
		std::string name;
		int width, height;
	};

	class Window
	{
	private:
		const WindowInfo m_info;
		GLFWwindow* m_window;

	public:
		Window(WindowInfo info, App* app);
		GLFWwindow* Get();
		void Destroy();
	};
}