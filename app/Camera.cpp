#include "Camera.h"
#include "Log.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float PI = 3.14159265f;
const float MAX_PITCH = PI/2 - 0.0001f;
const float MIN_PITCH = -PI/2 + 0.0001f;

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch) :
        mPosition(position), mWorldUp(worldUp), mYaw(yaw), mPitch(pitch)
{
    updateVectors();
}

Camera::~Camera() = default;

glm::vec3 Camera::getPosition() const
{
    return mPosition;
}

glm::mat4 Camera::getViewMatrix() const
{
    glm::vec3 front = getFront();
    glm::vec3 target(mPosition.x + front.x, mPosition.y + front.y, mPosition.z + front.z);
    return glm::lookAt(mPosition, target, mWorldUp);
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return mProjection;
}

glm::vec3 Camera::getFront() const
{
    return mFront;
}

glm::vec3 Camera::getRight() const
{
    return mRight;
}

glm::vec3 Camera::getUp() const
{
    return mUp;
}

glm::vec3 Camera::getWorldUp() const
{
    return mWorldUp;
}

void Camera::setPerspective(float fov, float aspect, float nearPlane, float farPlane)
{
    mFov = fov;
    mAspect = aspect;
    mNearPlane = nearPlane;
    mFarPlane = farPlane;
    mProjection = glm::perspective(mFov, mAspect, mNearPlane, mFarPlane);
}

void Camera::setAspect(float aspect)
{
    mAspect = aspect;
    mProjection = glm::perspective(mFov, mAspect, mNearPlane, mFarPlane);
}

void Camera::setAspect(int width, int height)
{
    mAspect = 1.0f * width / height;
    mProjection = glm::perspective(mFov, mAspect, mNearPlane, mFarPlane);
}

void Camera::setPosition(float x, float y, float z)
{
    mPosition = glm::vec3(x, y, z);
    updateVectors();
}

void Camera::setYawPitch(float yaw, float pitch)
{
    mYaw = yaw;
    mPitch = pitch;
    updateVectors();
}

void Camera::setYawPitchDegree(float yaw, float pitch)
{
    mYaw = glm::radians(yaw);
    mPitch = glm::clamp(pitch, MIN_PITCH, MAX_PITCH);
    updateVectors();
}

void Camera::setYaw(float rad)
{
    mYaw = rad;
    updateVectors();
}

void Camera::setPitch(float rad)
{
    mPitch = glm::clamp(rad, MIN_PITCH, MAX_PITCH);
    updateVectors();
}

void Camera::setYawDegree(float deg)
{
    mYaw = glm::radians(deg);
    updateVectors();
}

void Camera::setPitchDegree(float deg)
{
    setPitch(glm::radians(deg));
}

void Camera::lookAt(float x, float y, float z, float upX, float upY, float upZ)
{
    ASSERT(false, "Not yet implemented");
//    lookAt(glm::vec3(x, y, z), glm::vec3(upX, upY, upZ));
//
//    glm::vec3 dir(x - mPosition.x, y - mPosition.y, z - mPosition.z);
//    if (dir.x == 0 && dir.z == 0) {
//        if (dir.y > 0) {
//            mPitch = PI/2 - 0.01f;          // a little less than PI/2
//        } else if (dir.y < 0) {
//            mPitch = -PI/2 + 0.01f;         // a little greater than -PI/2
//        } else {
//            // do nothing
//        }
//        return;
//    }
//
//    float tmp = sqrtf(dir.x*dir.x + dir.z*dir.z);
//    mPitch = atan2f(dir.y, tmp);
//    mYaw = atan2f(dir.x, dir.z);
}

void Camera::lookAt(glm::vec3 target, glm::vec3)
{
    ASSERT(false, "Not yet implemented");
//    glm::vec3 dir(target.)
}

void Camera::updateVectors()
{
    // Calculate the new Front vector
    using namespace glm;

    glm::vec3 front;
    front.x = cos(mYaw) * cos(mPitch);
    front.y = sin(mPitch);
    front.z = sin(mYaw) * cos(mPitch);
    mFront = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    mRight = glm::normalize(glm::cross(mFront, mWorldUp));
    mUp = glm::normalize(glm::cross(mRight, mFront));
}

void Camera::move(glm::vec3 distance)
{
    mPosition = mPosition + distance;
}

void Camera::moveForward(float distance)
{
    glm::vec3 front = getFront();
    mPosition = glm::vec3(mPosition.x + front.x * distance, mPosition.y + front.y * distance, mPosition.z + front.z * distance);
}

void Camera::moveRight(float distance)
{
    glm::vec3 right = getRight();
    mPosition = glm::vec3(mPosition.x + right.x * distance, mPosition.y + right.y * distance, mPosition.z + right.z * distance);
}

void Camera::moveUp(float distance)
{
    glm::vec3 up = getWorldUp();
    mPosition = glm::vec3(mPosition.x + up.x * distance, mPosition.y + up.y * distance, mPosition.z + up.z * distance);
}

void Camera::turnRight(float rad)
{
    mYaw += rad;
    updateVectors();
}

void Camera::turnRightDegree(float deg)
{
    turnRight(glm::radians(deg));
    updateVectors();
}

void Camera::turnUp(float rad)
{
    mPitch -= rad;
    mPitch = glm::clamp(mPitch, MIN_PITCH, MAX_PITCH);
    updateVectors();
}

void Camera::turnUpDegree(float deg)
{
    turnUp(glm::radians(deg));
}
