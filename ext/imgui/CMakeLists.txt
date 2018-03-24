# Module Name
SET(MODULE_NAME ImGui)
SET(MODULE_EXPORT_NAME ${MODULE_NAME}Targets)

# Create library and alias for building as a sub-project
ADD_LIBRARY(${MODULE_NAME} STATIC "")
ADD_LIBRARY(${MODULE_NAME}::${MODULE_NAME} ALIAS ${MODULE_NAME})

# Add include directories
TARGET_INCLUDE_DIRECTORIES(${MODULE_NAME}
                           PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:>
                           )

TARGET_SOURCES(${MODULE_NAME}
               PRIVATE
               ${CMAKE_CURRENT_SOURCE_DIR}/imconfig.h
               ${CMAKE_CURRENT_SOURCE_DIR}/imgui.h
               ${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/imgui_convenience.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/imgui_convenience.h
               ${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/imgui_glfw.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/imgui_glfw.h
               ${CMAKE_CURRENT_SOURCE_DIR}/imgui_internal.h
               ${CMAKE_CURRENT_SOURCE_DIR}/stb_truetype.h
               ${CMAKE_CURRENT_SOURCE_DIR}/stb_rect_pack.h
               ${CMAKE_CURRENT_SOURCE_DIR}/stb_textedit.h
               )

# Require for config
TARGET_LINK_LIBRARIES(${MODULE_NAME} libcomputation)

TARGET_LINK_LIBRARIES(${MODULE_NAME} glLoadGen)

TARGET_LINK_LIBRARIES(${MODULE_NAME} glfw)
