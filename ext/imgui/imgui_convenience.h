/// OpenGL - by Carl Findahl - 2018

/* 
 * Utility functions to work with
 * ImGui more easily.
 */

#ifndef IMGUI_CONVENIENCE_H
#define IMGUI_CONVENIENCE_H

#include "linalg.h"

#include <vector>

#include "imgui.h"

namespace ImGui
{

void Value(const char* prefix, const vec2& v);

void Value(const char* prefix, const vec3& v);

void Value(const char* prefix, const vec4& v);

// Sourced from eliasdaler.github.io/using-imgui-with-sfml-pt2/
auto vector_of_string_getter(void* vec, int idx, const char** out_text);;

// Sourced from eliasdaler.github.io/using-imgui-with-sfml-pt2/
// Fill a combo box with the contents of a vector of strings
bool Combo(const char* label, int* currIndex, std::vector<std::string>& values);

// Sourced from eliasdaler.github.io/using-imgui-with-sfml-pt2/
// Fill a list box with the contents of a vector of strings
bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values);

/* Overload for using a vec3 with color edit. */        
bool ColorEdit3(const char* label, vec3& col, ImGuiColorEditFlags flags = 0);

/* Overload for using a vec3 with the color picker. */
bool ColorPicker3(const char* label, vec3& col, ImGuiColorEditFlags flags = 0);

/* Overload for using a vec4 with the color picker. */
bool ColorPicker4(const char* label, vec4& col, ImGuiColorEditFlags flags = 0, const float* ref_col = nullptr);

/* Overload for using a vec4 with color edit. */
bool ColorEdit4(const char* label, vec4& col, ImGuiColorEditFlags flags = 9);

 /* Input float that takes a vec2 */
bool InputFloat2(const char* label, vec2& vec, int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0);

/* Input float that takes a vec3 */
bool InputFloat3(const char* label, vec3& vec, int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0);

/* Input float that takes a vec4 */
bool InputFloat4(const char* label, vec4& vec, int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0);

/* Drag float that takes a vec2 */
bool DragFloat2(const char* label, vec2& vec, float v_min = 0.f, float v_max = 0.f);

/* Drag float that takes a vec3 */
bool DragFloat3(const char* label, vec3& vec, float v_speed = 1.f, float v_min = 0.f, float v_max = 0.f, const char* display_format = "%.3f", float power = 0.f);

/* Drag float that takes a vec3 */
bool DragFloat4(const char* label, vec4& vec, float v_speed = 1.f, float v_min = 0.f, float v_max = 0.f, const char* display_format = "%.3f", float power = 0.f);

} // ImGui

#endif // IMGUI_CONVENIENCE_H
