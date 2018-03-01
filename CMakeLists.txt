cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project("OpenGL")
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

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

# Until I bother with add_subdirectory, add those manually
add_executable(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/ext/gl/src/gl_cpp.cpp"
               "${CMAKE_SOURCE_DIR}/ext/gl/include/gl_cpp.hpp")
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

##############################################################################
# Resource Management
##############################################################################

# Move resource folder to binary dir
message(STATUS "Copying resource directory to binary path")
FILE(COPY "${CMAKE_SOURCE_DIR}/res" DESTINATION "${CMAKE_BINARY_DIR}")
