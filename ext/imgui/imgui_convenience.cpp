#include "imgui_convenience.h"

#include <vector>

void ImGui::Value(const char* prefix, const vec2& v)
{
    Text("%s: [%.2f, %.2f]", prefix, v[0], v[1]);
}

void ImGui::Value(const char* prefix, const vec4& v)
{
    Text("%s: [%.2f, %.2f, %.2f, %.2f]", prefix, v[0], v[1], v[2], v[3]);
}

void ImGui::Value(const char* prefix, const vec3& v)
{
    Text("%s: [%.2f, %.2f, %.2f]", prefix, v[0], v[1], v[2]);
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

bool ImGui::ColorEdit3(const char* label, vec3& col, ImGuiColorEditFlags flags /*= 0*/)
{
    return ImGui::ColorEdit3(label, col.data(), flags);
}

bool ImGui::ColorPicker3(const char* label, vec3& col, ImGuiColorEditFlags flags /*= 0*/)
{
    return ImGui::ColorPicker3(label, col.data(), flags);
}

bool ImGui::ColorPicker4(const char* label, vec4& col, ImGuiColorEditFlags flags /*= 0*/, const float* ref_col /*= nullptr*/)
{
    return ImGui::ColorPicker4(label, col.data(), flags, ref_col);
}

bool ImGui::ColorEdit4(const char* label, vec4& col, ImGuiColorEditFlags flags /*= 9*/)
{
    return ImGui::ColorEdit4(label, col.data(), flags);
}

bool ImGui::InputFloat2(const char* label, vec2& vec, int decimal_precision /*= -1*/, ImGuiInputTextFlags extra_flags /*= 0*/)
{
    return ImGui::InputFloat2(label, vec.data(), decimal_precision, extra_flags);
}

bool InputFloat3(const char* label, vec3& vec, int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0)
{
    return ImGui::InputFloat3(label, vec.data(), decimal_precision, extra_flags);
}

bool InputFloat4(const char* label, vec4& vec, int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0)
{
    return ImGui::InputFloat4(label, vec.data(), decimal_precision, extra_flags);
}

bool ImGui::DragFloat2(const char* label, vec2& vec, float v_min /*= 0.f*/, float v_max /*= 0.f*/)
{
    return ImGui::DragFloat2(label, vec.data(), 1.f, v_min, v_max, "%.2f");
}

bool ImGui::DragFloat3(const char* label, vec3& vec, float v_speed /*= 1.f*/, float v_min /*= 0.f*/, float v_max /*= 0.f*/, const char* display_format /*= "%.3f"*/, float power /*= 0.f*/)
{
    return ImGui::DragFloat3(label, vec.data(), v_speed, v_min, v_max, display_format, power);
}

bool ImGui::DragFloat4(const char* label, vec4& vec, float v_speed /*= 1.f*/, float v_min /*= 0.f*/, float v_max /*= 0.f*/, const char* display_format /*= "%.3f"*/, float power /*= 0.f*/)
{
    return ImGui::DragFloat4(label, vec.data(), v_speed, v_min, v_max, display_format, power);
}
