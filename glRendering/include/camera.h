/// OpenGLRendering - by Carl Findahl - 2018

/* 
 * The camera is a convenient way to create
 * a view matrix. It has a position and an
 * anchor point that it is looking at.
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

class Camera
{
private:
    // The point that the camera is looking at
    glm::vec3 mAnchorPoint;

    // The position of the camera node itself
    glm::vec3 mPosition;

public:
    Camera() = default;
    Camera(const glm::vec3& position, const glm::vec3& mAnchorPoint = {});

    // Move the camera by the vec3. Also moves the anchor
    void move(const glm::vec3& translationDelta);

    // Moves the anchor by the given position. Camera position stays put
    void moveAnchor(const glm::vec3& translationDelta);

    // Set the position of the camera anchor
    void setAnchor(const glm::vec3& newPosition);

    // Set the position of the camera. The anchor stays at the same relative position
    void setPosition(const glm::vec3& newPosition);

    // Return the viewMatrix produced by this camera
    glm::mat4 getViewMatrix() const;
};


#endif // CAMERA_H
