#include "shader.h"
#include "files.h"
#include "logging.h"
#include "gl_cpp.hpp"

#include <memory>

#include "glm/gtc/type_ptr.hpp"
#include "spdlog/fmt/fmt.h"


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

const unsigned Shader::name() const
{
    return mName;
}

void Shader::setUniform1f(const std::string& uniformName, float value)
{
    const auto location = getUniformLocation(uniformName);
    if (location != -1)
        gl::Uniform1f(location, value);
}

void Shader::setUniform2f(const std::string& uniformName, const glm::vec2& value)
{
    const auto location = getUniformLocation(uniformName);
    if (location != -1)
        gl::Uniform2fv(location, 1, glm::value_ptr(value));
}

void Shader::setUniform3f(const std::string& uniformName, const glm::vec3& value)
{
    const auto location = getUniformLocation(uniformName);
    if (location != -1)
        gl::Uniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::setUniform4f(const std::string& uniformName, const glm::vec4& value)
{
    const auto location = getUniformLocation(uniformName);
    if (location != -1)
        gl::Uniform4fv(location, 1, glm::value_ptr(value));
}

void Shader::setUniformMat2(const std::string& uniformName, const glm::mat2& value)
{
    const auto location = getUniformLocation(uniformName);
    if (location != -1)
        gl::UniformMatrix2fv(location, 1, gl::FALSE_, glm::value_ptr(value));
}

void Shader::setUniformMat3(const std::string& uniformName, const glm::mat3& value)
{
    const auto location = getUniformLocation(uniformName);
    if (location != -1)
        gl::UniformMatrix3fv(location, 1, gl::FALSE_, glm::value_ptr(value));
}

void Shader::setUniformMat4(const std::string& uniformName, const glm::mat4& value)
{
    const auto location = getUniformLocation(uniformName);
    if (location != -1)
        gl::UniformMatrix4fv(location, 1, gl::FALSE_, glm::value_ptr(value));
}

const unsigned Shader::compileShader(const std::string& sourceFile, unsigned type)
{
    // Load source code
    const auto& sourceStr = readFile(sourceFile);
    const auto* source = sourceStr.data();

    // If there is no source code
    if (sourceStr.empty())
    {
        logCustom()->error("Failed to load empty shader source: {}", sourceFile);
        return 0;
    }

    // Create Shader
    unsigned shader = gl::CreateShader(type);
    gl::ShaderSource(shader, 1, &source, nullptr);
    gl::CompileShader(shader);

    // If it failed to compile clean up
    if (!validateShaderCompilation(shader))
    {
        logErr("Shader has compile errors. See above!");
        gl::DeleteShader(shader);
        return 0;
    }

    return shader;
}

const bool Shader::validateShaderCompilation(unsigned shader)
{
    // Check if shader compiled
    int didCompile;
    gl::GetShaderiv(shader, gl::COMPILE_STATUS, &didCompile);

    // If not, print error
    if (!didCompile)
    {
        int length;
        gl::GetShaderiv(shader, gl::INFO_LOG_LENGTH, &length);
        std::unique_ptr<char> message(new char[length]);

        gl::GetShaderInfoLog(shader, length, nullptr, message.get());
        logCustom()->error("{}", message.get());
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

    // Error check the linking
    if (!validateProgramLinkage(mName))
    {
        gl::DeleteProgram(mName);
        logErr("Failed to link program. See errors above!");
    }

    // Cleanup
    gl::DeleteShader(vertexShader);
    gl::DeleteShader(fragmentShader);
}

const bool Shader::validateProgramLinkage(const unsigned program)
{
    // Check if program linked successfully
    int didLink;
    gl::GetProgramiv(program, gl::LINK_STATUS, &didLink);

    // If not, get error message and print it
    if (!didLink)
    {
        int length;
        gl::GetProgramiv(program, gl::INFO_LOG_LENGTH, &length);
        if (length)
        {
            std::unique_ptr<char> message(new char[length]);

            gl::GetProgramInfoLog(program, length, nullptr, message.get());
            logCustom()->error("{}", message.get());
        }
        else
        {
            logWarn("The error log is empty, but linker errors occured!");
        }
        return false;
    }

    return true;
}

const int Shader::getUniformLocation(const std::string& uniformName)
{
    // Check Cache
    if (mUniformCache.find(uniformName) != mUniformCache.end())
        return mUniformCache.at(uniformName);

    // Get Location
    const int location = gl::GetUniformLocation(mName, uniformName.data());

    // If invalid log error
    if (location == -1)
        logCustom()->warn("Uniform {} does not exist or is not in use!", uniformName);

    // Cache for future calls
    mUniformCache[uniformName] = location;

    return location;
}

