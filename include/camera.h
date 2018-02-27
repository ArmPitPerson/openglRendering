/// OpenGL Rendering by Carl Findahl (C) 2018

#ifndef CAMERA_H
#define CAMERA_H

#include "linalg.h"


class Camera
{
public:
    Camera() {}
    Camera(const vec4& position, const vec4& pointOfInterest);

    // Move camera by the given delta
    void move(const vec4& delta);

    // Get camera view matrix
    const mat4& viewMatrix() const;

private:
    // Called when the view matrix should update
    void computeViewMatrix();

private:
    // Position
    vec4 mPosition;

    // POI
    vec4 mPointOfInterest{ 0.f, 0.f, 5.f, 0.f };

    // View Matrix
    mat4 mViewMatrix = mat4::identity();

};


#endif // CAMERA_H
