#include "glfwInclude.h"
#include "glmInclude.h"

#include "App.h"
#include "Window.h"

int main() {
        
    Core::App app;

    app.Awake({ "Vulkan initiation", 800, 600 });
    app.Run();
    app.Destroy();

    return 0;
}