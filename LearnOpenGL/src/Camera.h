#pragma once

#include "defines.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement.
// Used as abstraction to stay away from window-system specific input methods.
enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UPWARDS,
    DOWNWARDS
};

// Default camera values
const f32 YAW = -90.0f;
const f32 PITCH = 0.0;
const f32 SPEED = 30.0f;
const f32 SENSITIVITY = 0.1f;
const f32 ZOOM = 45.0f;

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
    f32 m_Yaw;
    f32 m_Pitch;

    // camera options
    f32 m_MovementSpeed;
    f32 m_MouseSensitivity;
    f32 m_Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           f32 yaw = YAW, f32 pitch = PITCH);
    // constructor with scalar values
    Camera(f32 pos_x, f32 pos_y, f32 pos_z, f32 up_x, f32 up_y, f32 up_z, f32 yaw, f32 pitch);

    /**
     * @brief Get the View Matrix
     *
     * @return glm::mat4
     */
    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(CameraMovement direction, f32 delta_time);
    void ProcessMouseMovement(f32 xoffset, f32 yoffset, bool constrain_pitch = true);
    void ProcessMouseScroll(f32 yoffset);

private:
    void UpdateCameraVectors();
};
