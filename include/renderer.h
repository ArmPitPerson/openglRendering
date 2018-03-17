/// OpenGL - by Carl Findahl - 2018

/* 
 * The renderer is a unified utility to
 * draw various kinds of data passed to it.
 * For example, a RenderBatch can be passed,
 * and it will draw the entire batch. You can
 * also pass a single shape, which will just draw
 * the one shape. It's purpose is to streamline draw
 * calls.
 */

#ifndef RENDERER_H
#define RENDERER_H

class Curve;
class Shape2D;
class RenderBatch;
class VertexArray;

class Renderer
{
public:

    // Draw the provided data
    void draw(const Curve& curve) const;
    void draw(const Shape2D& shape) const;
    void draw(const RenderBatch& batch) const;
    void draw(const VertexArray& vao, const unsigned indexCount) const;

    // Draw the provided data with n instances
    void drawInstanced(const Shape2D& shape, const int instanceCount);
    void drawInstanced(const RenderBatch& batch, const int instanceCount);
    void drawInstanced(const VertexArray& vao, const unsigned indexCount, const int instanceCount);

};


#endif // RENDERER_H
