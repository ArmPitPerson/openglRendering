cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project("OpenGL-Rendering")

##############################################################################
# Varaibles
##############################################################################

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##############################################################################
# Main Application
##############################################################################

add_subdirectory(glRendering)

##############################################################################
# Libraries / Dependencies
##############################################################################

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
