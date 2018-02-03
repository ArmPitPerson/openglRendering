#include "glfwApplication.h"
#include "gl_cpp.hpp"
#include "GLFW/glfw3.h"
#include "vertexArray.h"
#include "serviceLocator.h"
#include "vertex.h"
#include "buffer.h"
#include "shader.h"
#include "linalg.h"


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
	Vertex vertices[] = { { -100.f, -100.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f },
			      { 100.f, -100.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f },
			      { 100.f, 100.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f },
			      { -100.f, 100.f, 0.f, 0.f, 0.f, 1.f, 0.0f, 1.f } };

	Vertex aertices[] = { { -300.f, -300.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f },
			      { -200.f, -300.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f },
			      { -200.f, -200.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f },
			      { -300.f, -200.f, 0.f, 0.f, 1.f, 1.f, 0.0f, 1.f } };

	unsigned indices[] = { 0, 1, 2, 2, 3, 0 };

	// Buffers and Arrays
	VertexBuffer vbo(vertices, sizeof(vertices));
	VertexBuffer vbo2(aertices, sizeof(aertices));
	IndexBuffer ibo(indices, sizeof(indices));
	VertexArray vao(vbo, ibo);
	vao.addAttribute(3, gl::FLOAT, offsetof(Vertex, x));
	vao.addAttribute(3, gl::FLOAT, offsetof(Vertex, r));
	vao.addAttribute(2, gl::FLOAT, offsetof(Vertex, u));

	Shader shader("vertex.vs", "frag.fs");
	shader.bind();
	
	mat4 transformMatrix = mat4::identity();
	transformMatrix(0, 0) = 1.f / 1280.f;
	transformMatrix(1, 1) = 1.f / 720.f;

	bool bUseBufferA = true;

	while (!glfwWindowShouldClose(mWindow)) {
		mInputManager.clear();
		glfwPollEvents();
		if (mInputManager.wasPressed(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(mWindow, true);
		if (mInputManager.wasPressed(GLFW_KEY_O, GLFW_KEY_I)) {
			if (bUseBufferA) {
				vao.setBuffer(vbo2);
				bUseBufferA = false;
			}
			else {
				vao.setBuffer(vbo);
				bUseBufferA = true;
			}
		}
		if (mInputManager.arePressed(GLFW_KEY_RIGHT)) {
			transformMatrix(0, 3) += 0.001f;
		}
		if (mInputManager.arePressed(GLFW_KEY_LEFT)) {
			transformMatrix(0, 3) -= 0.001f;
		}
		if (mInputManager.arePressed(GLFW_KEY_UP)) {
			transformMatrix(1, 3) += 0.001f;
		}
		if (mInputManager.arePressed(GLFW_KEY_DOWN)) {
			transformMatrix(1, 3) -= 0.001f;
		}

		gl::Clear(gl::COLOR_BUFFER_BIT);

		vao.bind();
		shader.setUniformMat4("uTransform", transformMatrix);
		gl::DrawElements(gl::TRIANGLES, 6, gl::UNSIGNED_INT, nullptr);

		glfwSwapBuffers(mWindow);
	}
}

