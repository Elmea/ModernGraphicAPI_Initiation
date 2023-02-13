#include <iostream>

#include "App.h"

using namespace Core;

App::App(WindowInfo windowInfo) 
{
    glfwInit();
    m_window = new Window(windowInfo, this);
    vkRenderer = new VkRenderer(m_window);
}

App::~App()
{
    delete vkRenderer;
    m_window->Destroy();
    delete m_window;
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

void App::MainLoop()
{
    while (!glfwWindowShouldClose(m_window->Get()))
    {
        glfwPollEvents();
        vkRenderer->UpdateUniformBuffer();
        vkRenderer->DrawFrame();
    }

    vkDeviceWaitIdle(vkRenderer->GetLogicalDevice());
}
