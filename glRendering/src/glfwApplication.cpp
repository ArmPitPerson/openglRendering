#include "glfwApplication.h"
#include "vertexArray.h"
#include "serviceLocator.h"
#include "uniformBlocks.h"
#include "logging.h"
#include "vertex.h"
#include "buffer.h"
#include "shader.h"
#include "texture.h"
#include "interpolation.h"
#include "randomEngine.h"
#include "files.h"
#include "clock.h"
#include "shapes.h"
#include "renderBatch.h"
#include "glfwCallbacks.h"

#include <array>

#include "gl_cpp.hpp"
#include "imgui.h"
#include "imgui_glfw.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"


GLFWApplication::GLFWApplication()
{
    ServiceLocator<InputManager>::provide(&mInputManager);
    glfwInit();

    // Context Hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Context Creation
    mWindow = glfwCreateWindow(1280, 720, "Open GL Rendering", nullptr, nullptr);
    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);

    // GLFW Callbacks
#ifndef NDEBUG
    glfwSetErrorCallback(error_callback);
#endif
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

    auto fontPath = getResourcePath("ProggyTiny.ttf");
    io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 10.f);

    gl::ClearColor(.2f, 0.3f, 0.3f, 1.0f);
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

    Shader basicShader(getResourcePath("vertex.vert"), getResourcePath("frag.frag"));
    basicShader.bind();

    Quad square({ 50.f, 50.f }, { 0.88f, 0.4f, 0.1f });
    Quad square2({ 50.f, 50.f }, { 0.9f, 0.1f, 0.3f });
    Quad square3({ 50.f, 50.f }, { 0.1f, 0.9f, 0.4f });

    glm::mat4 model1 = glm::rotate(glm::mat4(1.f), glm::radians(-55.f), glm::vec3(1.f, 0.f, 0.f));
    glm::mat4 model2 = glm::rotate(glm::mat4(1.f), glm::radians(-55.f), glm::vec3(1.f, 1.f, 0.f));
    glm::mat4 model3 = glm::rotate(glm::mat4(1.f), glm::radians(-55.f), glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -100.f));
    glm::mat4 proj = glm::perspective(glm::radians(60.f), 16.f / 9.f, 0.1f, 512.f);    

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
        if (mInputManager.arePressed(GLFW_KEY_W))
            view = glm::translate(view, glm::vec3(0.f, -1.f, 0.f));
        if (mInputManager.arePressed(GLFW_KEY_A))
            view = glm::translate(view, glm::vec3(1.f, 0.f, 0.f));
        if (mInputManager.arePressed(GLFW_KEY_S))
            view = glm::translate(view, glm::vec3(0.f, 1.f, 0.f));
        if (mInputManager.arePressed(GLFW_KEY_D))
            view = glm::translate(view, glm::vec3(-1.f, 0.f, 0.f));
        if (mInputManager.arePressed(GLFW_KEY_UP))
            view = glm::rotate(view, 0.05f, glm::vec3{ 1.f, 0.f, 0.f });
        if (mInputManager.arePressed(GLFW_KEY_DOWN))
            view = glm::rotate(view, -0.05f, glm::vec3{ 1.f, 0.f, 0.f });

        // Updating
        while (timeSinceUpdate > updateDelta)
        {
            timeSinceUpdate -= updateDelta;                  
        }

        // Application Drawing
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);             
        
        glm::mat4 mvpMatrix = proj * view * model1;
        basicShader.setUniformMat4("mvpMatrix", mvpMatrix);
        mRenderer.draw(square);

        mvpMatrix = proj * view * model2;
        basicShader.setUniformMat4("mvpMatrix", mvpMatrix);
        mRenderer.draw(square2);

        mvpMatrix = proj * view * model3;
        basicShader.setUniformMat4("mvpMatrix", mvpMatrix);
        mRenderer.draw(square3);

        // ImGui Drawing
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(mWindow);
    }
}
