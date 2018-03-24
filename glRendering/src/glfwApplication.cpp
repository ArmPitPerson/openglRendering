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
#include "texture.h"
#include "randomEngine.h"
#include "files.h"
#include "clock.h"
#include "shapes.h"
#include "renderBatch.h"
#include "glfwCallbacks.h"

#include "curve.h"

#include "imgui.h"
#include "imgui_glfw.h"

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
    Shader basicShader(getResourcePath("vertex.vs"), getResourcePath("frag.fs"));
    basicShader.bind();

    Shader curveShader(getResourcePath("curve"));

    UMatrices matrixUniforms;
    matrixUniforms.modelView = mat4::translate(0.f, 0.f, -1.5f);
    matrixUniforms.projection = mat4::orthographic(0.f, 1024.f, 0.f, 1024.f, -1.f, 1.f);

    mat4 cproj = mat4::orthographic(0.f, 1024.f, 0.f, 1024.f, -10.f, 10.f);

    UniformBuffer matrixBuffer(sizeof(UMatrices), basicShader);
    matrixBuffer.setUniformBlock("Matrices");
    matrixBuffer.setBlockData(&matrixUniforms, sizeof(UMatrices));
    matrixBuffer.bind(1);

    // Objects
    Circle circle{ 16.f, 64, vec3(0.85f, 0.06f, 0.33f) };
    
    // Renderer
    VertexArray batchVao;
    batchVao.addAttribute(3, gl::FLOAT, offsetof(Vertex, x));
    batchVao.addAttribute(3, gl::FLOAT, offsetof(Vertex, r));
    batchVao.addAttribute(2, gl::FLOAT, offsetof(Vertex, u));

    RenderBatch shapeBatch(std::move(batchVao));
    shapeBatch.push(circle);
    shapeBatch.clear();

    // Delta Time Measurement
    auto deltaClock = Clock{};
    auto updateDelta = Clock::TimeUnit{ 1.f / 144.f };
    auto timeSinceUpdate = Clock::TimeUnit{ 0.f };

    vec3 point1{ 200.f, 100.f };
    vec3 point2{ 1000.f, 500.f };
    vec3 point3{ 100.f, 1000.f };
    vec3 point4{ 700.f, 1000.f };

    Curve curve;
    curve.addControlPoint(point1);
    curve.addControlPoint(point2);
    curve.addControlPoint(point3);
    curve.addControlPoint(point4);
    curve.prepareDrawData();

    vec3 position;

    float interpolationFactor = 0.f;
    float velocity = 0.95f;

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
        if (mInputManager.arePressed(GLFW_KEY_D))
            matrixUniforms.modelView(0, 3) -= 25.f * deltaTime;
        if (mInputManager.arePressed(GLFW_KEY_A))
            matrixUniforms.modelView(0, 3) += 25.f * deltaTime;
        if (mInputManager.arePressed(GLFW_KEY_W))
            matrixUniforms.modelView(1, 3) -= 25.f * deltaTime;
        if (mInputManager.arePressed(GLFW_KEY_S))
            matrixUniforms.modelView(1, 3) += 25.f * deltaTime;
        if (mInputManager.arePressed(GLFW_KEY_E))
            matrixUniforms.modelView *= mat4::rotate(10.f * deltaTime, 1.f, 0.f, 0.f);
        if (mInputManager.arePressed(GLFW_KEY_Q))
            matrixUniforms.modelView *= mat4::rotate(-10.f * deltaTime, 1.f, 0.f, 0.f);
        
        ImGui::Text("FPS: %.2f", 1.f / deltaTime);
        ImGui::DragFloat3("Point 01", curve[0].data(), 1.f, 0.f, 1024.f);
        ImGui::DragFloat3("Point 02", curve[1].data(), 1.f, 0.f, 1024.f);
        ImGui::DragFloat3("Point 03", curve[2].data(), 1.f, 0.f, 1024.f);
        ImGui::DragFloat3("Point 04", curve[3].data(), 1.f, 0.f, 1024.f);

        // Updating
        while (timeSinceUpdate > updateDelta)
        {
            timeSinceUpdate -= updateDelta;
            
            interpolationFactor += velocity * updateDelta.count();

            if (interpolationFactor > 1.f)
            {
                interpolationFactor = 1.f;
                velocity = -velocity;
            }
            else if (interpolationFactor < 0.f)
            {
                interpolationFactor = 0.f;
                velocity = -velocity;
            }

            position = cubic(curve[0], curve[1], curve[2], curve[3], interpolationFactor);

            matrixUniforms.modelView(0, 3) = position[0];
            matrixUniforms.modelView(1, 3) = position[1];
            matrixUniforms.modelView(2, 3) = position[2];
            matrixBuffer.setPartialBlockData("modelView", matrixUniforms.modelView.data(), 64);
        }

        curve.prepareDrawData();

        ImGui::DragFloat("Velocity", &velocity, 0.01f, 0.f, 1.f, "%.2f");

        // Application Drawing
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
        
        curveShader.bind();
        curveShader.setUniformMat4("uProj", cproj);
        mRenderer.draw(curve);

        basicShader.bind();
        mRenderer.draw(circle);
        
        // ImGui Drawing
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(mWindow);
    }
}
