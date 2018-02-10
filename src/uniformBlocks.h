///  by Carl Findahl (C) 2018

#ifndef UNIFORMBLOCKS_H
#define UNIFORMBLOCKS_H

#include "linalg.h"

/// Each struct in this file prefixed with U correspond to a uniform layout in
/// OpenGL. They have proper padding and follow the std140 standard.

struct UMatrices {
	mat4 modelView;		// Offset 0
	mat4 projection;	// Offset 16
};


#endif // UNIFORMBLOCKS_H
