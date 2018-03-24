cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project("OpenGL-Rendering")

##############################################################################
# Varaibles
##############################################################################

set(cmake_cxx_standard 17)
set(cmake_cxx_standard REQUIRED ON)

# MSVC C++ 17
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
add_definitions(-D_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING)
endif()

##############################################################################
# Main Application
##############################################################################

add_subdirectory(glRendering)

##############################################################################
# Libraries / Dependencies
##############################################################################

# GLFW
add_subdirectory("ext/glfw")

# SPDLOG
set(INCLUDE_DIRS ${INCLUDE_DIRS} "ext/spdlog/include")

# GL LOAD GEN
include_directories(${INCLUDE_DIRS} "ext/gl/include")

# PCG
add_subdirectory("ext/PCG")

# ImGui
add_subdirectory("ext/imgui")

# Computation / Math Library
add_subdirectory("modules/computation")

# GL Load Gen
add_subdirectory("ext/gl")

# Utility Library
add_subdirectory("modules/utility")

# Find OpenGL
find_package(OpenGL)
