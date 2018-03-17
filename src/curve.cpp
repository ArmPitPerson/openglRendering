#include "curve.h"
#include "logging.h"
#include "interpolation.h"

Curve::Curve()
{
    mVao.addAttribute(3, gl::FLOAT, offsetof(Vertex, x));
    mVao.addAttribute(3, gl::FLOAT, offsetof(Vertex, r));
    mVao.addAttribute(2, gl::FLOAT, offsetof(Vertex, u));
}

void Curve::addControlPoint(const vec3& point)
{
    mControlPoints.push_back(point);
}

void Curve::setPresiscion(const int val)
{
    mPresiscion = val;
}

void Curve::prepareDrawData()
{
    mPoints.clear();

    auto stopAt = 16 * mPresiscion;
    for (int i = 0; i != stopAt; ++i)
    {
        vec3 position;
        if (mControlPoints.size() == 2)
        {
            position = lerp(mControlPoints[0], mControlPoints[1], i / static_cast<float>(stopAt - 1));
        }
        else if (mControlPoints.size() == 3)
        {
            position = quadratic(mControlPoints[0], mControlPoints[1],
                                 mControlPoints[2], i / static_cast<float>(stopAt - 1));
        }
        else if (mControlPoints.size() == 4)
        {
            position = cubic(mControlPoints[0], mControlPoints[1],
                             mControlPoints[2], mControlPoints[3],
                             i / static_cast<float>(stopAt - 1));
        }
        else
        {
            // #TODO Arbitrarily large curves
            logWarn("Curve currently supports a maximum of 4 control points.");
        }

        mPoints.push_back(Vertex{ position[0], position[1], position[2], 1.f, 1.f, 1.f, 0.f, 0.f });
    }

    mVbo = std::make_unique<VertexBuffer>(mPoints.data(), mPoints.size() * sizeof(Vertex));
    mVao.setBuffer(*mVbo);
}

const int Curve::getPointCount() const
{
    if (mControlPoints.size() > 4u)
    {
        logWarn("Drawing more than 4 points is not currently supported!");
    }
    return static_cast<int>(mControlPoints.size());
}

void Curve::bind() const
{
    mVao.bind();
}

void Curve::unbind() const
{
    mVao.unbind();
}
