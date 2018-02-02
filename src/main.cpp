#include "gl_cpp.hpp"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include "logging.h"
#include "shader.h"
#include "linalg.h"
#include <string>


struct Vertex {
	float x;
	float y;
	float z;

	float r;
	float g;
	float b;

	float u;
	float v;
};

void APIENTRY cppGLDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
	switch (severity) {
	case gl::DEBUG_SEVERITY_LOW:
	case gl::DEBUG_SEVERITY_NOTIFICATION:
		logCustom()->info("OpenGL Info: {}", message);
		break;
	case gl::DEBUG_SEVERITY_MEDIUM:
		logCustom()->warn("OpenGL Warning: {}", message);
		break;
	case gl::DEBUG_SEVERITY_HIGH:
		logCustom()->error("OpenGL Error: {}", message);
		break;
	default:
		logCustom()->error("OpenGL Unknown Severity: {}", message);
		break;
	}
}


int main() {
	glfwInit();	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Open GL Rendering", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	auto debugLog = spdlog::stdout_color_st("DEBUG");
	debugLog->set_pattern("[%H:%M:%S.%e] >> %v");

	Vertex vertices[] = {{-100.f, -100.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f},
			     {100.f, -100.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f},
			     {100.f, 100.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f},
			     {-100.f, 100.f, 0.f, 0.f, 0.f, 1.f, 0.0f, 1.f } };

	unsigned indices[] = { 0, 1, 2, 2, 3, 0 };

	// OpenGL Debug Messages
	unsigned unusedID = 0;
	gl::Enable(gl::DEBUG_OUTPUT_SYNCHRONOUS);
	gl::DebugMessageCallback(cppGLDebug, nullptr);
	gl::DebugMessageControl(gl::DONT_CARE, gl::DONT_CARE, gl::DONT_CARE, 0, &unusedID, gl::TRUE_);
	
	// Buffers and Arrays
	unsigned vbo;
	gl::CreateBuffers(1, &vbo);
	gl::NamedBufferStorage(vbo, sizeof(vertices), vertices, 0);

	unsigned ibo;
	gl::CreateBuffers(1, &ibo);
	gl::NamedBufferStorage(ibo, sizeof(indices), indices, 0);

	unsigned vao;
	gl::CreateVertexArrays(1, &vao);
	gl::VertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
	gl::VertexArrayElementBuffer(vao, ibo);

	gl::VertexArrayAttribBinding(vao, 0, 0);
	gl::VertexArrayAttribBinding(vao, 1, 0);
	gl::VertexArrayAttribBinding(vao, 2, 0);
	gl::VertexArrayAttribFormat(vao, 0, 3, gl::FLOAT, gl::FALSE_, offsetof(Vertex, x));
	gl::VertexArrayAttribFormat(vao, 1, 3, gl::FLOAT, gl::FALSE_, offsetof(Vertex, r));
	gl::VertexArrayAttribFormat(vao, 2, 2, gl::FLOAT, gl::FALSE_, offsetof(Vertex, u));
	gl::EnableVertexArrayAttrib(vao, 0);
	gl::EnableVertexArrayAttrib(vao, 1);
	gl::EnableVertexArrayAttrib(vao, 2);
		
	{
		Shader shader("vertex.vs", "frag.fs");
		shader.bind();

		double xMouse, yMouse;		
		vec2 mousePos;
		constexpr vec2 winSize{ 1280.f, 720.f };

		const mat4 transformMatrix = { 1 / 1280.f, 0, 0, 0, 0, 1 / 720.f, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

		// Main Loop
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			glfwGetCursorPos(window, &xMouse, &yMouse);
			mousePos = vec2(static_cast<float>(xMouse), static_cast<float>(-yMouse));

			gl::Clear(gl::COLOR_BUFFER_BIT);

			gl::BindVertexArray(vao);
			shader.setUniformMat4("uTransform", transformMatrix);
			gl::DrawElements(gl::TRIANGLES, 6, gl::UNSIGNED_INT, nullptr);

			glfwSwapBuffers(window);
		}

		// Cleanup
		gl::DeleteVertexArrays(1, &vao);
		gl::DeleteBuffers(1, &vbo);
	}

	glfwTerminate();
	return 0;
}
