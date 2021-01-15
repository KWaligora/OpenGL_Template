#include "Camera.h"

Camera::Camera(int WindowWidth, int windowHeight) 
{
	viewMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);

	radius = 8.0f;
	aspecRation = WindowWidth / windowHeight;
	FOV = 45.0f;
	near = 1.0f;
	far = 100.0f;
	theta = 0.0f;
	phi = 90.0f;
	cameraSpeed = 5.0f;
	zoom_factor = 1.1f;

	eye = glm::vec3(0.0f, 0.0f, 3.0f);
	center = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::SetViewMatrix()
{
	viewMatrix = glm::lookAt(eye, center, up);
}

void Camera::SetProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(FOV), aspecRation, near, far);
}

glm::mat4 Camera::GetViewMatrix() 
{
	return viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::GoForward(float deltaTime)
{
	eye += center * cameraSpeed * deltaTime;
}

void Camera::GoBack(float deltaTime)
{
	eye -= center * cameraSpeed * deltaTime;
}

void Camera::GoLeft(float deltaTime)
{
	eye -= glm::normalize(glm::cross(center, up)) * cameraSpeed * deltaTime;
}

void Camera::GoRight(float deltaTime)
{
	eye += glm::normalize(glm::cross(center, up)) * cameraSpeed * deltaTime;
}