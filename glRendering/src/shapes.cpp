#include "shapes.h"
#include "logging.h"

#include <cmath>

void Shape2D::bind() const
{
    if (!mGLData)
    {
        logErr("Shape2D does not have any data");
        return;
    }

    mGLData->vao.bind();
}

void Shape2D::unbind() const
{
    if (!mGLData)
    {
        logErr("Shape2D does not have any data");
        return;
    }

    mGLData->vao.unbind();
}

const unsigned Shape2D::getIndexCount() const
{
    if (mGLData)
    {
        return mGLData->ibo.getCount();
    }
    else
    {
        return 0;
    }
}

void Shape2D::addVertex(const glm::vec2& pos, const glm::vec3& col, const glm::vec2& tc)
{
    mVertices.push_back({ pos.x, pos.y, 0.f, col.r, col.g, col.b, tc.x, tc.y });
}

void Shape2D::addIndex(const unsigned idx)
{
    mIndices.push_back(idx);
}

void Shape2D::init()
{
    VertexBuffer vbo(mVertices.data(), mVertices.size() * sizeof(Vertex));
    IndexBuffer ibo(mIndices.data(), mIndices.size() * sizeof(unsigned), static_cast<unsigned>(mIndices.size()));
    
    VertexArray vao(vbo, ibo);
    vao.addAttribute(3, gl::FLOAT, offsetof(Vertex, x), false);
    vao.addAttribute(3, gl::FLOAT, offsetof(Vertex, r), false);
    vao.addAttribute(2, gl::FLOAT, offsetof(Vertex, u), false);

    mGLData = std::make_unique<ShapeGLData>(std::move(vbo), std::move(ibo), std::move(vao));
}

//////
/// RECTANGLE
//////

Quad::Quad(const glm::vec2& size, const glm::vec3& col)
{
    addVertex(size / -2.f, col, glm::vec2{ 0.f, 0.f });
    addVertex(glm::vec2{ size.x / 2.f, size.y / -2.f }, col, glm::vec2{ 1.f, 0.f });
    addVertex(size / 2.f, col, glm::vec2{ 1.f, 1.f });
    addVertex(glm::vec2{ size.x / -2.f, size.y / 2.f }, col, glm::vec2{ 0.f, 1.f });

    addIndices(0, 1, 2, 2, 3, 0);

    init();
}

Circle::Circle(const float radius, const unsigned points, const glm::vec3& col)
{
    // The center point
    addVertex(glm::vec2(0.f, 0.f), col, glm::vec2(0.5f, 0.5f));

    // #TODO Create a proper math header with consts like this
    constexpr float PI = 3.14156f;

    // Loop around the circle
    for (unsigned i = 0; i <= points; ++i)
    {
        // Add x-Coordinate
        auto x = std::cos((2 * PI / points) * i) * (radius * 2);
        auto y = std::sin((2 * PI / points) * i) * (radius * 2);

        // Add Texture Coordinates
        auto u = std::cos(((2 * PI / points) * i) + 1.f) / 2.f;
        auto v = std::sin(((2 * PI / points) * i) + 1.f) / 2.f;

        addVertex(glm::vec2(x, y), col, glm::vec2(u, v));
    }

    // Set up index buffer by walking the circle and stepping into the center for each triangle
    std::vector<unsigned> indices;
    for (unsigned i = 0; i <= points; ++i)
    {
        addIndices(0, i + 2, i + 1);
    }

    init();
}
