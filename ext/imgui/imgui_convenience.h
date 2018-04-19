/// OpenGL - by Carl Findahl - 2018

/* 
 * Utility functions to work with
 * ImGui more easily.
 */

#ifndef IMGUI_CONVENIENCE_H
#define IMGUI_CONVENIENCE_H

#include <vector>

#include "imgui.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace ImGui
{

void Value(const char* prefix, const glm::vec2& v);

void Value(const char* prefix, const glm::vec3& v);

void Value(const char* prefix, const glm::vec4& v);

// Sourced from eliasdaler.github.io/using-imgui-with-sfml-pt2/
auto vector_of_string_getter(void* vec, int idx, const char** out_text);;

// Sourced from eliasdaler.github.io/using-imgui-with-sfml-pt2/
// Fill a combo box with the contents of a vector of strings
bool Combo(const char* label, int* currIndex, std::vector<std::string>& values);

// Sourced from eliasdaler.github.io/using-imgui-with-sfml-pt2/
// Fill a list box with the contents of a vector of strings
bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values);

} // ImGui

#endif // IMGUI_CONVENIENCE_H
