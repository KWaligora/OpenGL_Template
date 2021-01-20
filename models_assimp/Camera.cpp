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

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}
// poruszanie "wsad"
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

    Position.y = 1.0f; //blokuje poruszanie w pionie / latanie
}
//poruszanie mysza
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

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    // wyznaczanie nowego wektora front
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // wyznaczanie nowych wektorow right i up
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

glm::vec3 Camera::getCameraPos()
{
    return Position;
}