#include "gl_cpp.hpp"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include "logging.h"
#include <sstream>
#include <iostream>
#include <fstream>
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


const std::string readFile(const std::string& filepath) {
	std::stringstream out;
	std::ifstream file(filepath);
	if (file.is_open()) {
		out << file.rdbuf();
		return out.str();
	}
	logCustom()->warn("Failed to read file: {}", filepath);
	return std::string("");
}


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

	Vertex vertices[] = {{-.5f, -.5f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f},
			     {.5f, -.5f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f},
			     {.5f, .5f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f},
			     {-.5f, .5f, 0.f, 0.f, 0.f, 1.f, 0.0f, 1.f } };

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

	// Create Shader
 	unsigned vertexShader = gl::CreateShader(gl::VERTEX_SHADER);
 	unsigned fragmentShader = gl::CreateShader(gl::FRAGMENT_SHADER);
 	unsigned program = gl::CreateProgram();
 
 	const auto& vertexSourceString = readFile("vertex.vs");
 	const auto* vertexSource = vertexSourceString.data();
	const auto& fragmentSourceString = readFile("frag.fs");
 	const auto* fragmentSource = fragmentSourceString.data();

 	gl::ShaderSource(vertexShader, 1, &vertexSource, nullptr);
 	gl::ShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
 
 	gl::CompileShader(vertexShader);
 	gl::CompileShader(fragmentShader);

	int out, len;
	gl::GetShaderiv(fragmentShader, gl::COMPILE_STATUS, &out);
	gl::GetShaderiv(fragmentShader, gl::INFO_LOG_LENGTH, &len);
	GLchar message[1024];
	gl::GetShaderInfoLog(vertexShader, 1024, &len, message);
	logCustom()->error("Shader Compile Error: {}", message);
 
 	gl::AttachShader(program, vertexShader);
 	gl::AttachShader(program, fragmentShader);
 	gl::LinkProgram(program);
 	gl::ValidateProgram(program);
 
 	gl::DeleteShader(vertexShader);
 	gl::DeleteShader(fragmentShader);
 	
 	gl::UseProgram(program);

	// Main Loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		gl::Clear(gl::COLOR_BUFFER_BIT);

		gl::BindVertexArray(vao);
		gl::DrawElements(gl::TRIANGLES, 6, gl::UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
	}

	// Cleanup
	gl::DeleteVertexArrays(1, &vao);
	gl::DeleteBuffers(1, &vbo);
	gl::DeleteProgram(program);
	glfwTerminate();
	return 0;
}
