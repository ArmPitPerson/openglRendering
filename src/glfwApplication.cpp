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
#include "image.h"
#include "files.h"

// GLFW Key Callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static InputManager* inputManager = ServiceLocator<InputManager>::get();
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
        inputManager->pressKey(key);
    else if (action == GLFW_RELEASE)
        inputManager->releaseKey(key);
}

GLFWApplication::GLFWApplication()
{
    ServiceLocator<InputManager>::provide(&mInputManager);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 2); // 2x MSAA
    mWindow = glfwCreateWindow(1280, 720, "Open GL Rendering", nullptr, nullptr);
    glfwSetKeyCallback(mWindow, key_callback);
    glfwMakeContextCurrent(mWindow);

    /// Print max textures
    int maxTextures;
    gl::GetIntegerv(gl::MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextures);
    logCustom()->info("Max bound textures: {}", maxTextures);
}

GLFWApplication::~GLFWApplication()
{
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void GLFWApplication::run()
{
    Vertex vertices[] = { // For a Plane
            { -1.f, -1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, },
            {  1.f,-1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, },
            {  1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, },
            { -1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 1.f, } };

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

    UMatrices uniformBlockData;
    UniformBuffer matrixBuffer(sizeof(UMatrices), shader);
    matrixBuffer.setUniformBlock("Matrices");
    matrixBuffer.bind();

    uniformBlockData.drawColor = { 0.2f, 0.1f, 0.1f, 1.f };

    RandomEngine rng;

    uniformBlockData.worldView = mat4::identity();
    uniformBlockData.projection = mat4::perspective(59.f, 1280.f / 720.f, 0.1f, 100.f);
    matrixBuffer.setBlockData(&uniformBlockData, sizeof(uniformBlockData));

    Texture koalaTex(getResourcePath("kaowa.png"), 3);
    Texture newTexture(koalaTex);
    koalaTex = newTexture;
    koalaTex.bind();
    
    Image drawTarget({ 1280, 720 }, EImageMode::ReadWrite);
    drawTarget.bind(0);

    gl::Enable(gl::DEPTH_TEST);
    gl::DepthFunc(gl::LEQUAL);

    while (!glfwWindowShouldClose(mWindow))
    {
        // Event Processing
        mInputManager.clear();
        glfwPollEvents();

        uniformBlockData.cursorPosition[2] = 0;

        // Input Handling
        if (mInputManager.wasPressed(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(mWindow, true);
        if (mInputManager.arePressed(GLFW_KEY_SPACE))
            uniformBlockData.cursorPosition[2] = 1;
        if (mInputManager.arePressed(GLFW_KEY_Q))
            uniformBlockData.drawColor[0] += 0.01f;
        if (mInputManager.arePressed(GLFW_KEY_A))
            uniformBlockData.drawColor[0] -= 0.01f;
        if (mInputManager.arePressed(GLFW_KEY_W))
            uniformBlockData.drawColor[1] += 0.01f;
        if (mInputManager.arePressed(GLFW_KEY_S))
            uniformBlockData.drawColor[1] -= 0.01f;
        if (mInputManager.arePressed(GLFW_KEY_E))
            uniformBlockData.drawColor[2] += 0.01f;
        if (mInputManager.arePressed(GLFW_KEY_D))
            uniformBlockData.drawColor[2] -= 0.01f;

        // Updating
        double x, y;
        glfwGetCursorPos(mWindow, &x, &y);
        uniformBlockData.cursorPosition = { x, y, uniformBlockData.cursorPosition[2] };
        matrixBuffer.setBlockData(&uniformBlockData, sizeof(uniformBlockData));

        // Drawing
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
        gl::DrawElements(gl::TRIANGLES, ibo.getCount(), gl::UNSIGNED_INT, nullptr);
        glfwSwapBuffers(mWindow);
    }
}
