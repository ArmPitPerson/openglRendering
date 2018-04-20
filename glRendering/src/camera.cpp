#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(const glm::vec3& position, const glm::vec3& anchorPoint) : mAnchorPoint(anchorPoint), mPosition(position)
{

}

void Camera::move(const glm::vec3& translationDelta)
{
    mPosition += translationDelta;
    mAnchorPoint += translationDelta;
}

void Camera::moveAnchor(const glm::vec3& translationDelta)
{
    mAnchorPoint += translationDelta;
}

void Camera::setAnchor(const glm::vec3& newPosition)
{
    mAnchorPoint = newPosition;
}

void Camera::setPosition(const glm::vec3& newPosition)
{
    const auto difference = mAnchorPoint - mPosition;
    mPosition = newPosition;
    mAnchorPoint = newPosition + difference;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(mPosition, mAnchorPoint, glm::vec3(0.f, 1.f, 0.f));
}
