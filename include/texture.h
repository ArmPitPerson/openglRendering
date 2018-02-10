///  by Carl Findahl (C) 2018

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>


class Texture {
public:
	Texture(const std::string& filepath);
	~Texture();

	void bind(const int bindingPoint = 0) const;

	void unbind() const;

private:
	// Load texture from file
	void loadFromFile(const std::string& filepath);

private:
	// OpenGL name
	unsigned mName = 0;

	// Bound to binding point
	mutable unsigned mBindingPoint = 0;

};


#endif // TEXTURE_H
