#include "glfwInclude.h"
#include "glmInclude.h"

#include "Window.h"
#include "App.h"

int main() {
        
    Core::App app;
    app.Awake({ "Vulkan initiation", 800, 600 });

    app.Update();

    return 0;
}