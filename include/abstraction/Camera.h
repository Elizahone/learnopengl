#pragma once
// It is a FPS-like camera system, and not suitable for one that need ROLL operation

#include <glad/glad.h>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const float YAW = -90.0;
static const float PITCH = 0.0;
static const float MOVEMENT_SPEED = 4.5f;
static const float MOUSE_SENSITIVE = 0.03f;
static const float FOV = 45.0f;

enum class CameraMovement {
    FOREWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

class Camera {
public:
    glm::vec3 position = glm::vec3(0);
    glm::vec3 front = glm::vec3(0, 0, -1); // the -z
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::vec3 world_up = glm::vec3(0, 1, 0);
    glm::vec3 right = glm::vec3(1, 0, 0);

    float pitch = PITCH;
    float yaw = YAW;
    float fov = FOV;

    float moveSpeed = MOVEMENT_SPEED;
    float mouseSensitive = MOUSE_SENSITIVE;

    Camera() = default;
    Camera(const glm::vec3& _position, const glm::vec3& _world_up = glm::vec3(0, 1, 0), float _pitch = PITCH, float _yaw = YAW, float _fov = FOV)
        : position(_position), world_up(_world_up), pitch(_pitch), yaw(_yaw), fov(_fov)
    {
        updateCameraVector();
    }

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void processKey(CameraMovement direction, float deltaTime) {
        float v = deltaTime * moveSpeed;
        switch(direction) {
            case CameraMovement::FOREWARD:
                position += v * front;
                break;
            case CameraMovement::BACKWARD:
                position -= v * front;
                break;
            case CameraMovement::LEFT:
                position -= v * right;
                break;
            case CameraMovement::RIGHT:
                position += v * right;
                break;
        }
    }

    void processMouse(float xoffset, float yoffset) {
        xoffset *= mouseSensitive;
        yoffset *= mouseSensitive;

        pitch += yoffset;
        yaw += xoffset;

        if (pitch > 89.0f) { pitch = 89.0f; }
        if (pitch < -89.f) { pitch = -89.f; }

        updateCameraVector();
    }

    void processScroll(float yoffset) {
        fov -= yoffset;

        if (fov < 1.0f) fov = 1.0f;
        if (fov > 75.0f) fov = 75.0f;
    }


private:
    void updateCameraVector() {
        using namespace glm;
        vec3 _front;
        _front.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
        _front.y = sin(glm::radians(this->pitch));
        _front.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
        this->front = normalize(_front);
        this->right = normalize(cross(this->front, this->world_up));
        this->up = normalize(cross(this->right, this->front));
    }
};
