#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement.
// Used as abstraction to stay away from window-system specific input methods.
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UPWARDS,
    DOWNWARDS
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
    // camera attributes
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;

    // euler angles
    float m_Yaw;
    float m_Pitch;

    // camera options
    float m_MovementSpeed;
    float m_MouseSensitivity;
    float m_Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // constructor with scalar values
    Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch);

    /**
     * @brief Get the View Matrix
     *
     * @return glm::mat4
     */
    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(CameraMovement direction, float delta_time);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrain_pitch = true);
    void ProcessMouseScroll(float yoffset);

private:
    void UpdateCameraVectors();
};
