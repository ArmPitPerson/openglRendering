#include "glfwApplication.h"
#include "gl_cpp.hpp"
#include "GLFW/glfw3.h"
#include "vertex.h"
#include "buffer.h"
#include "shader.h"
#include "linalg.h"


GLFWApplication::GLFWApplication() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	mWindow = glfwCreateWindow(1280, 720, "Open GL Rendering", nullptr, nullptr);
	glfwMakeContextCurrent(mWindow);
}

GLFWApplication::~GLFWApplication() {
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void GLFWApplication::run() {
	Vertex vertices[] = { { -100.f, -100.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f },
	{ 100.f, -100.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f },
	{ 100.f, 100.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f },
	{ -100.f, 100.f, 0.f, 0.f, 0.f, 1.f, 0.0f, 1.f } };

	unsigned indices[] = { 0, 1, 2, 2, 3, 0 };

	// Buffers and Arrays
	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, sizeof(indices));

	unsigned vao;
	gl::CreateVertexArrays(1, &vao);
	gl::VertexArrayVertexBuffer(vao, 0, vbo.name(), 0, sizeof(Vertex));
	gl::VertexArrayElementBuffer(vao, ibo.name());

	gl::VertexArrayAttribBinding(vao, 0, 0);
	gl::VertexArrayAttribBinding(vao, 1, 0);
	gl::VertexArrayAttribBinding(vao, 2, 0);
	gl::VertexArrayAttribFormat(vao, 0, 3, gl::FLOAT, gl::FALSE_, offsetof(Vertex, x));
	gl::VertexArrayAttribFormat(vao, 1, 3, gl::FLOAT, gl::FALSE_, offsetof(Vertex, r));
	gl::VertexArrayAttribFormat(vao, 2, 2, gl::FLOAT, gl::FALSE_, offsetof(Vertex, u));
	gl::EnableVertexArrayAttrib(vao, 0);
	gl::EnableVertexArrayAttrib(vao, 1);
	gl::EnableVertexArrayAttrib(vao, 2);

	Shader shader("vertex.vs", "frag.fs");
	shader.bind();
	
	mat4 transformMatrix = mat4::identity();
	transformMatrix(0, 0) = 1.f / 1280.f;
	transformMatrix(1, 1) = 1.f / 720.f;

	while (!glfwWindowShouldClose(mWindow)) {
		glfwPollEvents();

		gl::Clear(gl::COLOR_BUFFER_BIT);

		gl::BindVertexArray(vao);
		shader.setUniformMat4("uTransform", transformMatrix);
		gl::DrawElements(gl::TRIANGLES, 6, gl::UNSIGNED_INT, nullptr);

		glfwSwapBuffers(mWindow);
	}
	gl::DeleteVertexArrays(1, &vao);
}

