#include "Window.h"
#include "App.h"
#include "VkRenderer.h"

using namespace Core;

static void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
	app->SetFramebufferResized(true);
}

Window::Window(WindowInfo info, App* app) :
	m_info(info)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	m_window = glfwCreateWindow(info.width, info.height, info.name.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(m_window, app);
	glfwSetFramebufferSizeCallback(m_window, FramebufferResizeCallback);
}

GLFWwindow* Window::Get()
{
	return m_window;
}

void Window::Destroy()
{
	glfwDestroyWindow(m_window);
}