#include "glfwInclude.h"

#include "App.h"
#include "Window.h"

int main() {
        
    Core::App app{ { "Vulkan initiation", 800, 600 } };

    app.Run();

    return 0;
}