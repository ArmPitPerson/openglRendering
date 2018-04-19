# Module Name
SET(APPLICATION_NAME OpenGLRendering)
SET(APPLICATION_EXPORT_NAME ${APPLICATION_NAME}Targets)

# Create library and alias for building as a sub-project
ADD_EXECUTABLE(${APPLICATION_NAME} "")

# Add include directories
TARGET_INCLUDE_DIRECTORIES(${APPLICATION_NAME}
                           PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>
                           PRIVATE
                           "${CMAKE_CURRENT_SOURCE_DIR}"
                           "${CMAKE_SOURCE_DIR}/ext/spdlog/include"
                           )

# Add source files
TARGET_SOURCES(${APPLICATION_NAME}
               PRIVATE
               ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/include/enums.h
               ${CMAKE_CURRENT_SOURCE_DIR}/include/glfwApplication.h
               ${CMAKE_CURRENT_SOURCE_DIR}/src/glfwApplication.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/src/glfwCallbacks.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/include/image.h
               ${CMAKE_CURRENT_SOURCE_DIR}/src/image.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/include/inputManager.h
               ${CMAKE_CURRENT_SOURCE_DIR}/include/renderBatch.h
               ${CMAKE_CURRENT_SOURCE_DIR}/src/renderBatch.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/include/renderer.h
               ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/include/serviceLocator.h
               ${CMAKE_CURRENT_SOURCE_DIR}/include/shader.h
               ${CMAKE_CURRENT_SOURCE_DIR}/src/shader.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/include/shapes.h
               ${CMAKE_CURRENT_SOURCE_DIR}/src/shapes.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/include/stb_image.h
               ${CMAKE_CURRENT_SOURCE_DIR}/src/stb_image.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/include/texture.h
               ${CMAKE_CURRENT_SOURCE_DIR}/src/texture.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/include/textureView.h
               ${CMAKE_CURRENT_SOURCE_DIR}/src/textureView.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/include/uniformBlocks.h
               ${CMAKE_CURRENT_SOURCE_DIR}/include/vertex.h
               ${CMAKE_CURRENT_SOURCE_DIR}/include/vertexArray.h
               ${CMAKE_CURRENT_SOURCE_DIR}/src/vertexArray.cpp
               )

# Require / Link Libraries / Dependencies

# Find GLFW
find_package(glfw3 3.2 REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find OpenGL Mathematics
find_package(glm REQUIRED)

find_package(spdlog REQUIRED)

target_link_libraries(${APPLICATION_NAME}
                      spdlog::spdlog
                      OpenGL::GL
                      glfw
                      glm
                      )

TARGET_LINK_LIBRARIES(${APPLICATION_NAME} ImGui)

TARGET_LINK_LIBRARIES(${APPLICATION_NAME} glLoadGen)

TARGET_LINK_LIBRARIES(${APPLICATION_NAME} libcomputation::libcomputation)

TARGET_LINK_LIBRARIES(${APPLICATION_NAME} libutility::libutility)

# Set Install Targets
INSTALL(TARGETS ${APPLICATION_NAME} EXPORT ${APPLICATION_EXPORT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

# Move resource folder to binary dir
add_custom_command(
  TARGET ${APPLICATION_NAME} PRE_BUILD
  COMMAND python ${CMAKE_SOURCE_DIR}/resourceCopy.py ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/glRendering/res
  COMMENT "Copy resource files into ${CMAKE_BINARY_DIR}/res"
)

