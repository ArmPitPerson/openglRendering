cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project("OpenGL")

##############################################################################
# Varaibles
##############################################################################

set(cmake_cxx_standard 17)
set(cmake_cxx_standard REQUIRED ON)
set(INCLUDE_DIRS "include")
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(SOURCE_FILES "src/logging.cpp" "src/shader.cpp" "src/files.cpp"
				 "src/glfwApplication.cpp" "src/vertexArray.cpp"
				 "src/camera.cpp" "src/ext/stb_image.cpp" "src/texture.cpp"
                 "src/randomEngine.cpp")

# MSVC C++ 17
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
add_definitions(-D_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING)
endif()

##############################################################################
# Libraries
##############################################################################

# GLFW
add_subdirectory("ext/glfw")

# SPDLOG
set(INCLUDE_DIRS ${INCLUDE_DIRS} "ext/spdlog/include")

# GL LOAD GEN
include_directories(${INCLUDE_DIRS} "ext/gl/include")
set(SOURCE_FILES ${SOURCE_FILES} "ext/gl/src/gl_cpp.cpp")

# PCG
include_directories(${INCLUDE_DIRS} "ext/PCG")

##############################################################################
# Build Targets
##############################################################################

add_executable(${PROJECT_NAME} "src/main.cpp" ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

if(WIN32)
target_link_libraries(${PROJECT_NAME} opengl32.lib)
else()
target_link_libraries(${PROJECT_NAME} OpenGL)
endif()
