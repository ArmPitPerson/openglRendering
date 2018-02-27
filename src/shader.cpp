#include "shader.h"
#include "files.h"
#include "linalg.h"
#include "logging.h"
#include "gl_cpp.hpp"
#include "spdlog/fmt/fmt.h"
#include <memory>


Shader::Shader(const std::string& shaderName)
{
    const auto& vertFile = fmt::format("{}.vs", shaderName);
    const auto& fragFile = fmt::format("{}.fs", shaderName);

    unsigned vShader = compileShader(vertFile, gl::VERTEX_SHADER);
    unsigned fShader = compileShader(fragFile, gl::FRAGMENT_SHADER);

    if (vShader != 0 && fShader != 0)
        makeProgramAndCleanup(vShader, fShader);
}



Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned vShader = compileShader(vertexShader, gl::VERTEX_SHADER);
    unsigned fShader = compileShader(fragmentShader, gl::FRAGMENT_SHADER);

    if (vShader != 0 && fShader != 0)
        makeProgramAndCleanup(vShader, fShader);
}

Shader::~Shader()
{
    gl::DeleteProgram(mName);
}

void Shader::bind() const
{
    gl::UseProgram(mName);
}

void Shader::unbind() const
{
    gl::UseProgram(0);
}

void Shader::setUniform1f(const std::string& name, float value)
{
    const auto location = getUniformLocation(name);
    if (location != -1)
        gl::Uniform1f(location, value);
}

void Shader::setUniform2f(const std::string& name, const vec2& value)
{
    const auto location = getUniformLocation(name);
    if (location != -1)
        gl::Uniform2fv(location, 1, value.data());
}

void Shader::setUniform3f(const std::string& name, const vec3& value)
{
    const auto location = getUniformLocation(name);
    if (location != -1)
        gl::Uniform3fv(location, 1, value.data());
}

void Shader::setUniform4f(const std::string& name, const vec4& value)
{
    const auto location = getUniformLocation(name);
    if (location != -1)
        gl::Uniform4fv(location, 1, value.data());
}

void Shader::setUniformMat2(const std::string& name, const matM<float, 2>& value)
{
    const auto location = getUniformLocation(name);
    if (location != -1)
        gl::UniformMatrix2fv(location, 1, gl::FALSE_, value.data());
}

void Shader::setUniformMat3(const std::string& name, const matM<float, 3>& value)
{
    const auto location = getUniformLocation(name);
    if (location != -1)
        gl::UniformMatrix3fv(location, 1, gl::FALSE_, value.data());
}

void Shader::setUniformMat4(const std::string& name, const matM<float, 4>& value)
{
    const auto location = getUniformLocation(name);
    if (location != -1)
        gl::UniformMatrix4fv(location, 1, gl::FALSE_, value.data());
}

unsigned Shader::compileShader(const std::string& sourceFile, unsigned type)
{
    // Load source code
    const auto& sourceStr = readFile(sourceFile);
    const auto* source = sourceStr.data();

    // If there is no source code
    if (sourceStr.empty())
    {
        logCustom()->error("Failed to load shader source: {}", sourceFile);
        return 0;
    }

    // Create Shader
    unsigned shader = gl::CreateShader(type);
    gl::ShaderSource(shader, 1, &source, nullptr);
    gl::CompileShader(shader);

    // If it failed to compile
    if (!validateShaderCompilation(shader))
    {
        logCustom()->error("Shader has compile errors. See above!");
        gl::DeleteShader(shader);
        return 0;
    }

    return shader;
}

bool Shader::validateShaderCompilation(unsigned shader)
{
    // Check if shader compiled
    int didCompile;
    gl::GetShaderiv(shader, gl::COMPILE_STATUS, &didCompile);

    // If not, print error
    if (didCompile != gl::TRUE_)
    {
        int length;
        gl::GetShaderiv(shader, gl::INFO_LOG_LENGTH, &length);
        std::unique_ptr<char> message(new char[length]);
        gl::GetShaderInfoLog(shader, length, &length, message.get());
        logCustom()->error("Shader Compile Error: {}", message.get());
        return false;
    }
    return true;
}

void Shader::makeProgramAndCleanup(const unsigned vertexShader, const unsigned fragmentShader)
{
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

int Shader::getUniformLocation(const std::string& name)
{
    // Check Cache
    if (mUniformCache.find(name) != mUniformCache.end())
        return mUniformCache.at(name);

    // Get Location
    const int location = gl::GetUniformLocation(mName, name.data());

    // If invalid log error
    if (location == -1)
        logCustom()->warn("Uniform {} does not exist or is not in use!", name);

    // Cache for future calls
    mUniformCache[name] = location;

    return location;
}

