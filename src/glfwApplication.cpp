#include "glfwApplication.h"
#include "gl_cpp.hpp"
#include "GLFW/glfw3.h"
#include "vertexArray.h"
#include "serviceLocator.h"
#include "vertex.h"
#include "buffer.h"
#include "shader.h"
#include "linalg.h"


// GLFW Key Callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	static InputManager* inputManager = ServiceLocator<InputManager>::get();
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
		inputManager->pressKey(key);
	else if (action == GLFW_RELEASE)
		inputManager->releaseKey(key);
}


GLFWApplication::GLFWApplication() {
	ServiceLocator<InputManager>::provide(&mInputManager);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	mWindow = glfwCreateWindow(1280, 720, "Open GL Rendering", nullptr, nullptr);
	glfwSetKeyCallback(mWindow, key_callback);
	glfwMakeContextCurrent(mWindow);
}

GLFWApplication::~GLFWApplication() {
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void GLFWApplication::run() {
	Vertex vertices[] = { { -1280.f, -720.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f },
			      {  1280.f, -720.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f },
			      {  1280.f,  720.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f },
			      { -1280.f,  720.f, 0.f, 0.f, 1.f, 1.f, .0f, 1.f } };

	unsigned indices[] = { 0, 1, 2, 2, 3, 0 };

	// Buffers and Arrays
	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, sizeof(indices), 6);
	VertexArray vao(vbo, ibo);
	vao.addAttribute(3, gl::FLOAT, offsetof(Vertex, x));
	vao.addAttribute(3, gl::FLOAT, offsetof(Vertex, r));
	vao.addAttribute(2, gl::FLOAT, offsetof(Vertex, u));
	
	Shader shader("vertex.vs", "frag.fs");
	shader.bind();
	
	UniformBuffer worldBlock(160, shader.name());
	worldBlock.setUniformBlock("WorldInfo");
	float windowSize[2] = { 1280.f, 720.f };
	worldBlock.bind();
	worldBlock.setBlockData("windowSize", windowSize, sizeof(windowSize));
	
	mat4 transformMatrix = mat4::identity();
	transformMatrix(0, 0) = 1.f / 1280.f;
	transformMatrix(1, 1) = 1.f / 720.f;

	double mX, mY;
	vec2 mousePos;

	while (!glfwWindowShouldClose(mWindow)) {
		mInputManager.clear();
		glfwPollEvents();
		float glTime = static_cast<float>(glfwGetTime());
		glfwGetCursorPos(mWindow, &mX, &mY);
		mousePos[0] = mX; mousePos[1] = mY;
		worldBlock.setBlockData("time", &glTime, sizeof(glTime));
		worldBlock.setBlockData("mousePos", mousePos.data(), 2 * sizeof(float));
		worldBlock.setBlockData("transformMatrix", transformMatrix.data(), 16 * sizeof(float));

		if (mInputManager.wasPressed(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(mWindow, true);
		
		gl::Clear(gl::COLOR_BUFFER_BIT);
		vao.bind();
		gl::DrawElements(gl::TRIANGLES, ibo.getCount(), gl::UNSIGNED_INT, nullptr);

		glfwSwapBuffers(mWindow);
	}
}

