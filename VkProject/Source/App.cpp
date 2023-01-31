#include "App.h"

using namespace Core;

void App::Awake(WindowInfo windowInfo) 
{
    glfwInit();
    m_window = new Window(windowInfo);
}

void App::Start() {}

void App::Update() 
{
    while (!glfwWindowShouldClose(m_window->Get()))
    {
        glfwPollEvents();
    }
}

void App::Destroy() 
{
    m_window->Destroy();
    delete m_window;
    glfwTerminate();
}