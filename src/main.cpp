#include "gl_cpp.hpp"
#include "spdlog/spdlog.h"
#include "glfwApplication.h"
#include "logging.h"
#include <string>


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
	auto debugLog = spdlog::stdout_color_st("DEBUG");
	debugLog->set_pattern("[%H:%M:%S.%e] >> %v");

	GLFWApplication application;

	// OpenGL Debug Messages
	unsigned unusedID = 0;
	gl::Enable(gl::DEBUG_OUTPUT_SYNCHRONOUS);
	gl::DebugMessageCallback(cppGLDebug, nullptr);
	gl::DebugMessageControl(gl::DONT_CARE, gl::DONT_CARE, gl::DONT_CARE, 0, &unusedID, gl::TRUE_);

	application.run();
	
	return 0;
}