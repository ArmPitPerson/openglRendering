#include "glfwApplication.h"
#include "gl_cpp.hpp"
#include "GLFW/glfw3.h"
#include "vertexArray.h"
#include "serviceLocator.h"
#include "uniformBlocks.h"
#include "logging.h"
#include "vertex.h"
#include "buffer.h"
#include "shader.h"
#include "texture.h"
#include "randomEngine.h"
#include "files.h"
#include "clock.h"
#include "shapes.h"
#include "renderBatch.h"
#include "glfwCallbacks.h"

#include "imgui.h"
#include "imgui_glfw.h"
#include "glm/vec2.hpp"

#include <array>
#include "interpolation.h"

GLFWApplication::GLFWApplication()
{
    ServiceLocator<InputManager>::provide(&mInputManager);
    glfwInit();

    // Context Hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 2); // 2x MSAA

    // Context Creation
    mWindow = glfwCreateWindow(1024, 1024, "Open GL Rendering", nullptr, nullptr);
    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);

    // GLFW Callbacks
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(mWindow, key_callback);
    glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
    glfwSetCursorPosCallback(mWindow, cursor_position_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);

    // GL Setup
    gl::Enable(gl::DEPTH_TEST);
    gl::DepthFunc(gl::LEQUAL);
    gl::PointSize(2.f);

    // ImGui Setup
    mImGuiContext = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfwGL3_Init(mWindow);
    ImGui::StyleColorsDark();

    //auto fontPath = getResourcePath("ProggyTiny.ttf");
    //io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 10.f);
}

GLFWApplication::~GLFWApplication()
{
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext(mImGuiContext);
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void GLFWApplication::run()
{
    Clock deltaClock;
    auto updateDelta = Clock::TimeUnit{ 1.f / 144.f };
    auto timeSinceUpdate = Clock::TimeUnit{};

    while (!glfwWindowShouldClose(mWindow))
    {
        // Clock Update
        const auto deltaTime = deltaClock.restart().count();
        timeSinceUpdate += Clock::TimeUnit{ deltaTime };

        // Event Processing
        mInputManager.clear();
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        // Input Handling
        if (mInputManager.wasPressed(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(mWindow, true);

        // Updating
        while (timeSinceUpdate > updateDelta)
        {
            timeSinceUpdate -= updateDelta;            
        }

        // Application Drawing
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);             
        
        // ImGui Drawing
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(mWindow);
    }
}
