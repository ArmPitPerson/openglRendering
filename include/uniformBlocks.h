///  by Carl Findahl (C) 2018

#ifndef UNIFORMBLOCKS_H
#define UNIFORMBLOCKS_H

#include "linalg.h"

/// Each struct in this file prefixed with U correspond to a uniform layout in
/// OpenGL. They have proper padding and follow the std140 standard.

struct UMatrices {
	mat4 modelWorld;        // Offset 0
    mat4 worldView;	        // Offset 64
	mat4 projection;	    // Offset 128
    vec4 drawColor;         // Offset 192
    vec4i cursorPosition;   // Offset 208
};


#endif // UNIFORMBLOCKS_H
