#include "camera.h"


Camera::Camera(const vec4& position, const vec4& pointOfInterest) : mPosition(position), mPointOfInterest(pointOfInterest)
{
    computeViewMatrix();
}

void Camera::move(const vec4& delta)
{
    mPosition += delta;
    mPointOfInterest += delta;
    computeViewMatrix();
}

const mat4& Camera::viewMatrix() const
{
    return mViewMatrix;
}

void Camera::computeViewMatrix()
{
    // Compute Vectors
    const vec4 fwdVector = (mPointOfInterest - mPosition) / length(mPointOfInterest - mPosition);
    const vec4 sideVector = cross(fwdVector, vec4({ 0, 1, 0, 0 }));
    const vec4 cameraUpVector = cross(sideVector, fwdVector);

    // Make Matrix
    mViewMatrix(0, 0) = sideVector[0];
    mViewMatrix(1, 0) = sideVector[1];
    mViewMatrix(2, 0) = sideVector[2];
    mViewMatrix(0, 1) = cameraUpVector[0];
    mViewMatrix(1, 1) = cameraUpVector[1];
    mViewMatrix(2, 1) = cameraUpVector[2];
    mViewMatrix(0, 2) = fwdVector[0];
    mViewMatrix(1, 2) = fwdVector[1];
    mViewMatrix(2, 2) = fwdVector[2];
    mViewMatrix(0, 3) = -mPosition[0];
    mViewMatrix(1, 3) = -mPosition[1];
    mViewMatrix(2, 3) = -mPosition[2];
    mViewMatrix(3, 3) = 1.f;
}
