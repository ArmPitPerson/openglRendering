/// GLFW_Testing by Carl Findahl (C) 2017
/// A Kukon Project

#ifndef GLHELPER_H
#define GLHELPER_H

#include <string>

void clearGLErrors();

void logGLErrors();

std::string readFile(const std::string& filepath);


#endif // GLHELPER_H
