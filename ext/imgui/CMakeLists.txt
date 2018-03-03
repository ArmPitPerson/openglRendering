# Custom CMake File for IMGUI - Unofficial
# Files are added to main project because this is what ImGui Recommends

MESSAGE(STATUS "Adding ImGui version 1.53")
target_sources(${PROJECT_NAME}
            PRIVATE
            "${CMAKE_SOURCE_DIR}/ext/imgui/imgui.h"
            "${CMAKE_SOURCE_DIR}/ext/imgui/imgui.cpp"
            "${CMAKE_SOURCE_DIR}/ext/imgui/imconfig.h"
            "${CMAKE_SOURCE_DIR}/ext/imgui/imgui_internal.h"
            "${CMAKE_SOURCE_DIR}/ext/imgui/imgui_draw.cpp"
            "${CMAKE_SOURCE_DIR}/ext/imgui/imgui_glfw.h"
            "${CMAKE_SOURCE_DIR}/ext/imgui/imgui_glfw.cpp"
            "${CMAKE_SOURCE_DIR}/ext/imgui/imgui_convenience.h"
            "${CMAKE_SOURCE_DIR}/ext/imgui/imgui_convenience.cpp"
            # "${CMAKE_SOURCE_DIR}/ext/imgui/extra_fonts/binary_to_compressed_c.cpp"
            # "${CMAKE_SOURCE_DIR}/ext/imgui/stb_rect_pack.h"
            # "${CMAKE_SOURCE_DIR}/ext/imgui/stb_textedit.h"
            # "${CMAKE_SOURCE_DIR}/ext/imgui/stb_truetype.h"
            )

