///  by Carl Findahl (C) 2018
/// A Kukon Development Project

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "buffer.h"


class VertexArray {
public:
	VertexArray();
	VertexArray(const VertexBuffer& vbo);
	VertexArray(const VertexBuffer& vbo, const IndexBuffer& ibo);
	~VertexArray();

	void bind() const;

	void unbind() const;

	void addAttribute(int size, unsigned type, unsigned offset, bool normalize = false);

	void removeLastAttribute();

	void setBuffer(const VertexBuffer& vbo);

	void setIndexBuffer(const IndexBuffer& ibo);

private:
	// The OpenGL Name
	unsigned mName = 0;

	// Next Attribute to set
	unsigned mNextAttribute = 0;

};


#endif // VERTEXARRAY_H
