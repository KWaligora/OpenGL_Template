#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>

// definiuje opcje poruszania "wsad"
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
private:
    void UpdateCameraVectors();

public:
    // wektory
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // euler
    float Yaw;
    float Pitch;

    // wlasciwosci kamery
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera();

    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(Camera_Movement direction, float deltaTime); // poruszanie "wsad"
    void ProcessMouseMovement(float xoffset, float yoffset); // poruszanie mysza
    glm::vec3 GetCameraPos();

};

