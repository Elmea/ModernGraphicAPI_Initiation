#pragma once
#include <string>

#include "glfwInclude.h"

namespace Core
{
	class Window
	{
	private:
		std::string m_name;
		int m_width, m_height;
		GLFWwindow* m_window;

	public:
		Window(std::string name, int width, int height);
		GLFWwindow* Get();
		void Destroy();
	};
}