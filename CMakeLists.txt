cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project("OpenGL")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

##############################################################################
# Varaibles
##############################################################################

set(cmake_cxx_standard 17)
set(cmake_cxx_standard REQUIRED ON)
set(INCLUDE_DIRS "include")

# MSVC C++ 17
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
add_definitions(-D_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING)
endif()

##############################################################################
# Build Targets
##############################################################################

add_executable(${PROJECT_NAME} "ext/gl/src/gl_cpp.cpp" "ext/gl/include/gl_cpp.h") # Until I bother with add_subdirectory
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

##############################################################################
# Libraries
##############################################################################

# GLFW
add_subdirectory("ext/glfw")

# SPDLOG
set(INCLUDE_DIRS ${INCLUDE_DIRS} "ext/spdlog/include")

# GL LOAD GEN
include_directories(${INCLUDE_DIRS} "ext/gl/include")

# PCG
include_directories(${INCLUDE_DIRS} "ext/PCG")

include("src/CMakeLists.txt")
include("include/CMakeLists.txt")

if(WIN32)
target_link_libraries(${PROJECT_NAME} "opengl32.lib")
else()
target_link_libraries(${PROJECT_NAME} "OpenGL")
endif()
