#include <iostream>

#include "App.h"

using namespace Core;

App::App(WindowInfo windowInfo) 
{
    glfwInit();
    m_window = new Window(windowInfo, this);
    vkRenderer = new VkRenderer(m_window);
    camera = new Camera(windowInfo.width, windowInfo.height, 0.001f, -10.f, 10.f, -10.f, 10.f, 0.1f, 1000.f, 60.f);
}

App::~App()
{
    delete vkRenderer;
    m_window->Destroy();
    delete m_window;
    delete camera;
}

void App::Run()
{
    try
    {
        vkRenderer->InitVulkan();
        MainLoop();
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << std::endl;
        return;
    }
}

void App::ProcessInput()
{
	GLFWwindow* window = m_window->Get();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
		glfwSetWindowShouldClose(window, true);
        return;
    }

    if (!mouseCaptured)
    {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
        {
            double newMouseX, newMouseY;
            glfwGetCursorPos(window, &newMouseX, &newMouseY);

            inputs.mouseX = newMouseX;
            inputs.mouseY = newMouseY;

            mouseCaptured = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    else
    {
        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        {
            if (mouseCaptured)
            {
                mouseCaptured = false;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }

        inputs.forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
        inputs.backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
        inputs.left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
        inputs.right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
        inputs.up = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
        inputs.down = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

        double newMouseX, newMouseY;
        glfwGetCursorPos(window, &newMouseX, &newMouseY);
        inputs.mouseDeltaX = newMouseX - inputs.mouseX;
        inputs.mouseDeltaY = newMouseY - inputs.mouseY;

        inputs.mouseX = newMouseX;
        inputs.mouseY = newMouseY;
    }
}


void App::MainLoop()
{
    while (!glfwWindowShouldClose(m_window->Get()))
    {
        glfwPollEvents();
		ProcessInput();
        camera->Update(inputs);
        vkRenderer->UpdateUniformBuffer(rotationSpeed, *camera);
        vkRenderer->DrawFrame();
    }

    vkDeviceWaitIdle(vkRenderer->GetLogicalDevice());
}
