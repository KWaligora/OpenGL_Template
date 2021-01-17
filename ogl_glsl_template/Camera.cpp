#include "Camera.h"

Camera::Camera() 
    :Front(glm::vec3(0.0f, 0.0f, -1.0f))
{
	Position = glm::vec3(0.0f, 1.0f, -3.0f);
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    Yaw = 90.0f;
    Pitch = 0.0f;;

    MovementSpeed = 2.5f;
    MouseSensitivity = 0.1f;
    Zoom = 45.0f;

    UpdateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;

    Position.y = 1.0f;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}

glm::vec3 Camera::getCameraPos()
{
    return Position;
}