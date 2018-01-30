#include "shader.h"
#include "files.h"
#include "logging.h"
#include "gl_cpp.hpp"
#include "spdlog/fmt/fmt.h"


Shader::Shader(const std::string& shaderName) {
	const auto& vertFile = fmt::format("{}.vs", shaderName);
	const auto& fragFile = fmt::format("{}.fs", shaderName);
	
	unsigned vShader = compileShader(vertFile, gl::VERTEX_SHADER);
	unsigned fShader = compileShader(fragFile, gl::FRAGMENT_SHADER);

	if (vShader != 0 && fShader != 0)
		makeProgramAndCleanup(vShader, fShader);
}



Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned vShader = compileShader(vertexShader, gl::VERTEX_SHADER);
	unsigned fShader = compileShader(fragmentShader, gl::FRAGMENT_SHADER);

	if (vShader != 0 && fShader != 0)
		makeProgramAndCleanup(vShader, fShader);
}

Shader::~Shader() {
	gl::DeleteProgram(mName);
}

void Shader::bind() const {
	gl::UseProgram(mName);
}

void Shader::unbind() const {
	gl::UseProgram(0);
}

unsigned Shader::compileShader(const std::string& sourceFile, unsigned type) {
	// Load source code
	const auto& sourceStr = readFile(sourceFile);
	const auto* source = sourceStr.data();

	// If there is no source code
	if (sourceStr.empty()) {
		logCustom()->error("Failed to load shader source: {}", sourceFile);
		return 0;
	}

	// Create Shader
	unsigned shader = gl::CreateShader(type);
	gl::ShaderSource(shader, 1, &source, nullptr);
	gl::CompileShader(shader);

	// If it failed to compile
	if (!validateShaderCompilation(shader)) {
		logCustom()->error("Shader has compile errors. See above!");
		gl::DeleteShader(shader);
		return 0;
	}

	return shader;
}

bool Shader::validateShaderCompilation(unsigned shader) {
	// Check if shader compiled
	int didCompile;
	gl::GetShaderiv(shader, gl::COMPILE_STATUS, &didCompile);

	// If not, print error
	if (didCompile != gl::TRUE_) {
		int length;
		gl::GetShaderiv(shader, gl::INFO_LOG_LENGTH, &length);
		std::unique_ptr<char> mMessage(new char[length + 1]);
		gl::GetShaderInfoLog(shader, length + 1, &length, mMessage.get());
		logCustom()->error("Shader Compile Error: {}", *mMessage);
		return false;
	}
	return true;
}

void Shader::makeProgramAndCleanup(const unsigned vertexShader, const unsigned fragmentShader) {
	mName = gl::CreateProgram();

	// Attach and link
	gl::AttachShader(mName, vertexShader);
	gl::AttachShader(mName, fragmentShader);
	gl::LinkProgram(mName);
	gl::ValidateProgram(mName);
	
	// Cleanup
	gl::DeleteShader(vertexShader);
	gl::DeleteShader(fragmentShader);
}

