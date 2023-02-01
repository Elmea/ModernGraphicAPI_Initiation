#include "Window.h"

using namespace Core;

Window::Window(WindowInfo info) :
	m_info(info)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = glfwCreateWindow(info.width, info.height, info.name.c_str(), nullptr, nullptr);
}

GLFWwindow* Window::Get()
{
	return m_window;
}

void Window::Destroy()
{
	glfwDestroyWindow(m_window);
}