#include "glfwApplication.h"
#include "logging.h"

#include <string>
#include <iostream>

#include "gl_cpp.hpp"
#include "spdlog/spdlog.h"

void APIENTRY cppGLDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
	switch (severity) {
	case gl::DEBUG_SEVERITY_NOTIFICATION:
		logCustom()->debug("OpenGL Debug Info: {}", message);
		break;
	case gl::DEBUG_SEVERITY_LOW:
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
    debugLog->info("Application Start Entry");

	GLFWApplication application;

#ifndef NDEBUG
	// OpenGL Debug Messages
	unsigned unusedID = 0;
	gl::Enable(gl::DEBUG_OUTPUT_SYNCHRONOUS);
	gl::DebugMessageCallback(cppGLDebug, nullptr);
	gl::DebugMessageControl(gl::DONT_CARE, gl::DONT_CARE, gl::DONT_CARE, 0, &unusedID, gl::TRUE_);
#endif

	application.run();
	
    debugLog->info("Application Terminate Entry");
	return 0;
}