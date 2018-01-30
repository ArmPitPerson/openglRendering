///  by Carl Findahl (C) 2018
/// A Kukon Development Project

#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <string>


template<typename T, unsigned M>
class vecM;


class Shader final {
public:
	Shader(const std::string& shaderName);
	Shader(const std::string& vertexShader, const std::string& fragmentShader);
	~Shader();

	void bind() const;

	void unbind() const;

	void setUniform1f(const std::string& name, float value);

	void setUniform2f(const std::string& name, const vecM<float, 2>& value);

	void setUniform3f(const std::string& name, const vecM<float, 3>& value);

	void setUniform4f(const std::string& name, const vecM<float, 4>& value);


private:
	unsigned compileShader(const std::string& sourceFile, unsigned type);

	//************************************
	// Method:    Shader::validateShaderCompilation
	// Access:    private 
	// Parameter: unsigned shader
	// Brief:     Validate compilation, true if all is good, false if error
	//************************************
	bool validateShaderCompilation(unsigned shader);

	void makeProgramAndCleanup(const unsigned vertexShader, const unsigned fragmentShader);

	int getUniformLocation(const std::string& name);

private:
	// OpenGL Name
	unsigned mName = 0;

	// Cache of Uniform Locations
	std::map<std::string, int> mUniformCache;

};


#endif // SHADER_H
