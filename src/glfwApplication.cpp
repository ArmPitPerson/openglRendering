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
#include "linalg.h"
#include "camera.h"
#include "texture.h"
#include "randomEngine.h"
#include "textureView.h"
#include "image.h"
#include "files.h"
#include "clock.h"

#include "imgui.h"
#include "imgui_glfw.h"

// ImGui extern for input management
extern bool g_MouseJustPressed[3];

// GLFW Key Callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static InputManager* inputManager = ServiceLocator<InputManager>::get();

    ImGuiIO& io = ImGui::GetIO();
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if(!io.WantCaptureKeyboard) // If ImGui is not reserving input
            inputManager->pressKey(key);
        io.KeysDown[key] = true; // ImGui
        logCustom()->debug("Pressed key: {}", key);
    }
    else if (action == GLFW_RELEASE)
    {
        inputManager->releaseKey(key);
        io.KeysDown[key] = false; // ImGui
        logCustom()->debug("Released key: {}", key);
    }

    (void)mods; // Modifiers are not reliable across systems
    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}

// GLFW Mouse Button Callback
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    static InputManager* inputManager = ServiceLocator<InputManager>::get();
    ImGuiIO& io = ImGui::GetIO();

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if(!io.WantCaptureMouse) // If ImGui is not reserving input
            inputManager->pressKey(button);
        logCustom()->debug("Pressed mouse button: {}", button);

        // ImGui Interaction
        if (button >= 0 && button < 3)
            g_MouseJustPressed[button] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        inputManager->releaseKey(button);
        logCustom()->debug("Released mouse button: {}", button);
    }
}

// GLFW Cursor Position Callback
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    static InputManager* inputManager = ServiceLocator<InputManager>::get();
    inputManager->moveCursor({ xpos, ypos });
    logCustom()->debug("Cursor at: X[{}] Y[{}]", xpos, ypos);
}

// GLFW Mouse Scroll Wheel Callback
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    static InputManager* inputManager = ServiceLocator<InputManager>::get();
    inputManager->scrollMouse({ xoffset, yoffset });
    logCustom()->debug("Scrolldelta: X[{}] Y[{}]", xoffset, yoffset);

    // ImGui Interaction
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheel += (float)yoffset;
}

// GLFW Error Callback
static void error_callback(int error, const char* description)
{
    logCustom()->error("GLFW Error #{}: {}", error, description);
}

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
    mWindow = glfwCreateWindow(1280, 720, "Open GL Rendering", nullptr, nullptr);
    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(0);

    // GLFW Callbacks
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(mWindow, key_callback);
    glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
    glfwSetCursorPosCallback(mWindow, cursor_position_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);

    // ImGui Setup
    mImGuiContext = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfwGL3_Init(mWindow);
    ImGui::StyleColorsDark();

    auto fontPath = getResourcePath("ProggyTiny.ttf");
    io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 10.f);
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
    Vertex vertices[] = { // For a Plane
            { -1.f, -1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f },
            {  1.f, -1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f },
            {  1.f,  1.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f },
            { -1.f,  1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 1.f } };

    unsigned indices[] = { 0, 1, 2, 2, 3, 0 };

    // Buffers and Arrays
    VertexBuffer vbo(vertices, sizeof(vertices));
    IndexBuffer ibo(indices, sizeof(indices), 36);
    VertexArray vao(vbo, ibo);
    vao.addAttribute(3, gl::FLOAT, offsetof(Vertex, x));
    vao.addAttribute(3, gl::FLOAT, offsetof(Vertex, r));
    vao.addAttribute(2, gl::FLOAT, offsetof(Vertex, u));
    vao.bind();

    // Uniforms and Shaders
    Shader shader(getResourcePath("vertex.vs"), getResourcePath("frag.fs"));
    shader.bind();

//     UAppData appUniforms;
//     appUniforms.frameBufferSize = { 1280.f, 720.f };
//     appUniforms.cursorPosition = mInputManager.getCursorPosition();
// 
//     UniformBuffer appBuffer(sizeof(UAppData), shader);
//     appBuffer.setUniformBlock("AppData");
//     appBuffer.bind(1);
//     appBuffer.setBlockData(&appBuffer, sizeof(UAppData));

    UMatrices matrixUniforms;
    matrixUniforms.modelView = mat4::translate(0.f, 0.f, -1.5f);
    matrixUniforms.projection = mat4::perspective(59.f, 1280.f / 720.f, 0.1f, 100.f);

    UniformBuffer matrixBuffer(sizeof(UMatrices), shader);
    matrixBuffer.setUniformBlock("Matrices");
    matrixBuffer.bind();
    matrixBuffer.setBlockData(&matrixUniforms, sizeof(UMatrices));

    gl::Enable(gl::DEPTH_TEST);
    gl::DepthFunc(gl::LEQUAL);

    // Texture View
    Texture exampleTex(getResourcePath("example.png"), 5);
    TextureView exampleView(exampleTex, 5);
    exampleView.bind();

    bool rotate = true;

    // Delta Time Measurement
    auto deltaClock = Clock{};
    auto deltaTime = Clock::TimeUnit{ 1.f / 144.f };
    auto timeSinceUpdate = Clock::TimeUnit{ 0 };

    while (!glfwWindowShouldClose(mWindow))
    {
        // Event Processing
        mInputManager.clear();
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        // Input Handling
        if (mInputManager.wasPressed(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(mWindow, true);
        if (mInputManager.arePressed(GLFW_KEY_W))
            matrixUniforms.modelView(2, 3) += .1f * deltaTime.count();
        else if (mInputManager.arePressed(GLFW_KEY_S))
            matrixUniforms.modelView(2, 3) -= .1f * deltaTime.count();
        else if (mInputManager.arePressed(GLFW_KEY_D))
            matrixUniforms.modelView(0, 3) += .1f * deltaTime.count();
        else if (mInputManager.arePressed(GLFW_KEY_A))
            matrixUniforms.modelView(0, 3) -= .1f * deltaTime.count();

        // Clock Update
        auto frameTime = deltaClock.restart();
        timeSinceUpdate += frameTime;

        ImGui::Text("FPS: %.1f", 1.f / frameTime.count());
        if (ImGui::Button("Trilinear")){
            exampleView.setFilterMode(ETextureFilterMode::Trilinear);
        }
        ImGui::SameLine();
        if (ImGui::Button("Nearest Nearest"))
        {
            exampleView.setFilterMode(ETextureFilterMode::NearestNearest);
        }
        ImGui::SameLine();
        if (ImGui::Button("Nearest Linear"))
        {
            exampleView.setFilterMode(ETextureFilterMode::NearestLinear);
        }
        ImGui::SameLine();
        if (ImGui::Button("Linear Nearest"))
        {
            exampleView.setFilterMode(ETextureFilterMode::LinearNearest);
        }
        ImGui::Checkbox("Enable Rotation", &rotate);

        // Updating
        while (timeSinceUpdate > deltaTime)
        {
            timeSinceUpdate -= deltaTime;
//             appUniforms.timeSinceStart = static_cast<float>(glfwGetTime());
//             appUniforms.cursorPosition = mInputManager.getCursorPosition();
//             appBuffer.setBlockData(&appUniforms, sizeof(UAppData));
            if (rotate)
                matrixUniforms.modelView *= mat4::rotate(45.f * deltaTime.count(), 0.f, 1.f, 0.f);
            matrixBuffer.setBlockData(&matrixUniforms, sizeof(UMatrices));
        }

        // Drawing
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
        gl::DrawElements(gl::TRIANGLES, ibo.getCount(), gl::UNSIGNED_INT, nullptr);
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(mWindow);
    }
}
