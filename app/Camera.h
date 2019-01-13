#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * Camera is a class represents
 */
class Camera {
public:
    glm::vec3 mPosition;
    glm::vec3 mWorldUp;
    float mYaw;
    float mPitch;

    float mFov;
    float mAspect;
    float mNearPlane;
    float mFarPlane;

    glm::mat4 mProjection;

    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;

    explicit Camera(glm::vec3 position = glm::vec3(0.f), glm::vec3 worldUp = glm::vec3(0.f, 1.0f, 0.0f), float yaw = 0.f, float pitch = 0.f);

    ~Camera();

    glm::vec3 getPosition() const;

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix() const;

    glm::vec3 getFront() const;

    glm::vec3 getRight() const;

    glm::vec3 getUp() const;

    glm::vec3 getWorldUp() const;

    void setPerspective(float fov, float aspect, float nearPlane, float farPlane);

    void setAspect(float aspect);

    void setAspect(int width, int height);

    void setPosition(float x, float y, float z);

    void setYawPitch(float yaw, float pitch);

    void setYawPitchDegree(float yaw, float pitch);

    void setYaw(float rad);

    void setPitch(float rad);

    void setYawDegree(float deg);

    void setPitchDegree(float deg);

    void move(glm::vec3 distance);

    void moveForward(float distance);

    void moveRight(float distance);

    void moveUp(float distance);

    void turnRight(float rad);

    void turnRightDegree(float deg);

    void turnUp(float rad);

    void turnUpDegree(float deg);

    void lookAt(float x, float y, float z, float upX, float upY, float upZ);

    void lookAt(glm::vec3 target, glm::vec3 up);

private:
    void updateVectors();

};


