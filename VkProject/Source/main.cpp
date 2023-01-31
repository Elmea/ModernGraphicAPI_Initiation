#include "glfwInclude.h"
#include "glmInclude.h"

#include "Window.h"

int main() {
    glfwInit();

    Core::Window window { "Vulkan initiation", 800, 600 };

    while (!glfwWindowShouldClose(window.Get())) 
    {
        glfwPollEvents();
    }

    window.Destroy();
    glfwTerminate();

    return 0;
}