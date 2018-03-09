///  by Carl Findahl (C) 2018
/// A Kukon Development Project

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "buffer.h"


class VertexArray
{
public:
    VertexArray();
    VertexArray(const VertexBuffer& vbo);
    VertexArray(const VertexBuffer& vbo, const IndexBuffer& ibo);

    VertexArray(VertexArray&& other);

    VertexArray& operator=(VertexArray&& other);

    ~VertexArray();

    // Bind Vertex Array
    void bind() const;

    // Unbind Vertex Array
    void unbind() const;

    // Get the OpenGL name of the Vertex Array
    const unsigned name() const;

    // Add an attribute to the vertex array. For a vec4, you would add(4, gl::FLOAT, ...)
    void addAttribute(int size, unsigned type, unsigned offset, bool normalize = false);
    
    // Add a NOT normalized integral vertex attribute. Type must be of an integral type.
    void addIntegerAttribute(int size, unsigned type, unsigned offset);

    // Remove and disable the last attribute
    void removeLastAttribute();

    // Set the vertex buffer that has the Vertex Array attribute data
    void setBuffer(const VertexBuffer& vbo);
    
    // Set the index buffer to use for indexed drawing
    void setIndexBuffer(const IndexBuffer& ibo);

private:
    // The OpenGL Name
    unsigned mName = 0;

    // Next Attribute to set
    unsigned mNextAttribute = 0;

};


#endif // VERTEXARRAY_H
