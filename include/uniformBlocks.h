///  Open GL - Carl Findahl - 2018

/*
 * This file contains structs that correspond with uniform
 * blocks in the various GLSL shaders used in this project
 * to easily be able to set uniform data and pass it to 
 * OpenGL.
 */

#ifndef UNIFORMBLOCKS_H
#define UNIFORMBLOCKS_H

#include "linalg.h"

/// Each struct in this file prefixed with U correspond to a uniform layout in
/// OpenGL. They have proper padding and follow the std140 standard.

struct UMatrices {
	mat4 modelView;            // Offset 0
	mat4 projection;	        // Offset 128
};

struct UAppData
{
    vec2 cursorPosition;        // Offset 0
    vec2 frameBufferSize;       // Offset 8
    float timeSinceStart;       // Offset 16
};

#endif // UNIFORMBLOCKS_H
