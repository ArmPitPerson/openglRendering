/// OpenGL - Carl Findahl - 2018

/*
 * Shader contains an abstraction of 
 * an OpenGL program with a vertex and
 * fragment shader component.
 */

#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <string>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat2x2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"

class Shader final
{
public:
    Shader(const std::string& shaderName);
    Shader(const std::string& vertexShader, const std::string& fragmentShader);
    ~Shader();

    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;

    // Bind Shader to context
    void bind() const;

    // Unbind Shader from context
    void unbind() const;

    // Get the OpenGL name
    const unsigned name() const;

    void setUniform1f(const std::string& uniformName, float value);

    void setUniform2f(const std::string& uniformName, const glm::vec2& value);

    void setUniform3f(const std::string& uniformName, const glm::vec3& value);

    void setUniform4f(const std::string& uniformName, const glm::vec4& value);

    void setUniformMat2(const std::string& uniformName, const glm::mat2& value);

    void setUniformMat3(const std::string& uniformName, const glm::mat3& value);

    void setUniformMat4(const std::string& uniformName, const glm::mat4& value);


private:
    // Compile and error check the provided source code of type (VERTEX or FRAGMENT shader)
    const unsigned compileShader(const std::string& sourceFile, unsigned type);

    // Validate compilation, true if all is good
    const bool validateShaderCompilation(unsigned shader);

    // Attach shaders, link and validate the program
    void makeProgramAndCleanup(const unsigned vertexShader, const unsigned fragmentShader);

    // Validate linking, true if all is good
    const bool validateProgramLinkage(const unsigned program);

    const int getUniformLocation(const std::string& uniformName);

private:
    // OpenGL Name
    unsigned mName = 0;

    // Cache of Uniform Locations
    std::map<std::string, int> mUniformCache;

};


#endif // SHADER_H
