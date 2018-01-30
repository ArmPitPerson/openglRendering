#include "glUtility.h"
#include "Logging.h"

#include <fstream>
#include <sstream>

#include "gl_cpp.hpp"


void clearGLErrors() {
	while (gl::GetError());
}

void logGLErrors() {
	while (GLenum error = gl::GetError()) {
		logCustom()->error("OpenGL Error: {0}", error);
	}
}

std::string readFile(const std::string& filepath) {
	// Open file
	std::ifstream file(filepath);
	std::stringstream outstream;
	
	// Read contents
	outstream << file.rdbuf();
	return outstream.str();
}
