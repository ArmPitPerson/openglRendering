#include "imgui_convenience.h"

#include <vector>

void ImGui::Value(const char* prefix, const glm::vec2& v)
{
    Text("%s: [%.2f, %.2f]", prefix, v.x, v.y);
}

void ImGui::Value(const char* prefix, const glm::vec4& v)
{
    Text("%s: [%.2f, %.2f, %.2f, %.2f]", prefix, v.x, v.y, v.z, v.w);
}

void ImGui::Value(const char* prefix, const glm::vec3& v)
{
    Text("%s: [%.2f, %.2f, %.2f]", prefix, v.x, v.y, v.z);
}

auto ImGui::vector_of_string_getter(void* vec, int idx, const char** out_text)
{
    auto& vector = *static_cast<std::vector<std::string>*>(vec);
    if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
    *out_text = vector.at(idx).c_str();
    return true;
}

bool ImGui::Combo(const char* label, int* currIndex, std::vector<std::string>& values)
{
    if (values.empty()) { return false; }
    return Combo(label, currIndex, vector_of_string_getter, static_cast<void*>(&values), static_cast<int>(values.size()));
}

bool ImGui::ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
{
    if (values.empty()) { return false; }
    return ListBox(label, currIndex, vector_of_string_getter, static_cast<void*>(&values), static_cast<int>(values.size()));
}
