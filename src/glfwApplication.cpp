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
    mWindow = glfwCreateWindow(1920, 1080, "Open GL Rendering", glfwGetPrimaryMonitor(), nullptr);
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
            { -.5f, -.5f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, },
            {  .5f, -.5f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, },
            {  .5f,  .5f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, },
            { -.5f,  .5f, 0.f, 1.f, 1.f, 1.f, 0.f, 1.f, } };

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
    Shader shader("vertex.vs", "frag.fs");
    shader.bind();

    UMatrices uniformBlockData;
    UniformBuffer matrixBuffer(sizeof(UMatrices), shader);
    matrixBuffer.setUniformBlock("Matrices");
    matrixBuffer.bind();

    RandomEngine rng;

    uniformBlockData.worldView = mat4::translate(0.f, 0.f, -1.5f);
    uniformBlockData.projection = mat4::perspective(59.f, 1280.f / 720.f, 0.1f, 100.f);
    for(int i = 0; i != 64; ++i)
        uniformBlockData.modelWorld[i] = mat4::translate(rng.uniform(-6.0, 6.0), -2.0f + rng.uniform(-5.0, 0.5), rng.uniform(-0.25, 0.6));
    matrixBuffer.setBlockData(&uniformBlockData, sizeof(uniformBlockData));

    Image drawTarget({ 1920.f, 1080.f }, EImageMode::WriteOnly);
    drawTarget.bind();

    Texture image("karray.png", 1, 12);
    image.bind();

    gl::Enable(gl::DEPTH_TEST);
    gl::DepthFunc(gl::LEQUAL);

    while (!glfwWindowShouldClose(mWindow))
    {
        // Event Processing
        mInputManager.clear();
        glfwPollEvents();

        // Input Handling
        if (mInputManager.wasPressed(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(mWindow, true);
//         if (mInputManager.arePressed(GLFW_KEY_W))
//             uniformBlockData.worldView(1, 3) += 0.1f;
//         if (mInputManager.arePressed(GLFW_KEY_S))
//             uniformBlockData.worldView(1, 3) -= 0.1f;
//         if (mInputManager.arePressed(GLFW_KEY_D))
//             uniformBlockData.worldView(0, 3) += 0.1f;
//         if (mInputManager.arePressed(GLFW_KEY_A))
//             uniformBlockData.worldView(0, 3) -= 0.1f;

        // Updating
        matrixBuffer.setPartialBlockData("worldView", uniformBlockData.worldView.data(), 64);

        // Drawing
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
        for (int i = 0; i != 64; ++i)
        {
            uniformBlockData.kaowaIndex = i;
            uniformBlockData.modelWorld[i] *= mat4::translate(0.f, 0.01f, 0.f);
            if (uniformBlockData.modelWorld[i](1, 3) > 3.5f)
            {
                uniformBlockData.modelWorld[i](0, 3) = rng.uniform(-6.0, 6.0);
                uniformBlockData.modelWorld[i](1, 3) = rng.uniform(-5.0, -3.0);
                uniformBlockData.modelWorld[i](2, 3) = rng.uniform(-0.25, 0.65);
            }
            matrixBuffer.setBlockData(&uniformBlockData, sizeof(uniformBlockData));
            gl::DrawElements(gl::TRIANGLES, ibo.getCount(), gl::UNSIGNED_INT, nullptr);
        }

        glfwSwapBuffers(mWindow);
    }
}
