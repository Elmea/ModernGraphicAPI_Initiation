#include "Window.h"

using namespace Core;

Window::Window(std::string name, int width, int height) :
	m_name(name), m_width(width), m_height(height)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
}

GLFWwindow* Window::Get()
{
	return m_window;
}

void Window::Destroy()
{
	glfwDestroyWindow(m_window);
}