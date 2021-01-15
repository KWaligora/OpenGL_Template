#include "Camera.h"

Camera::Camera(int WindowWidth, int windowHeight) 
{
	viewMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::mat4(1.0f);

	radius = 8.0f;
	aspecRation = WindowWidth / windowHeight;
	FOV = 45.0f;
	near = 1.0f;
	far = 100.0f;
	theta = 0.0f;
	phi = 90.0f;
	ROT_STEP = 5.0f;
	zoom_factor = 1.1f;

	eye = glm::vec3(0.0f, 0.0f, radius);
	center = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::SetViewMatrix()
{
	viewMatrix = glm::lookAt(eye, center, up);
}

void Camera::SetViewMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	viewMatrix = glm::lookAt(eye, center, up);

	Camera::eye = eye;
	Camera::center = center;
	Camera::up = up;
}

void Camera::SetProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(FOV), aspecRation, near, far);
}

void Camera::SetProjectionMatrix(float fov, float aspecRatio, float near, float far)
{
	projectionMatrix = glm::perspective(glm::radians(FOV), aspecRatio, near, far);

	Camera::FOV = fov;
	Camera::aspecRation = aspecRatio;
	Camera::near = near;
	Camera::far = far;
}

glm::mat4 Camera::GetViewMatrix() 
{
	return viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::UpdateEyePosition()
{
	Camera::theta = theta;
	Camera::phi = phi;

	eye.x = center.x + radius * glm::sin(glm::radians(theta)) * glm::sin(glm::radians(phi));
	eye.y = center.y + radius * glm::cos(glm::radians(phi));
	eye.z = center.z + radius * glm::cos(glm::radians(theta)) * glm::sin(glm::radians(phi));
}

void Camera::UpdateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(FOV), aspecRation, near, far);
}

void Camera::TurnLeft() 
{
	theta += ROT_STEP;
	if (theta > 360.0f)
		theta -= 360.0f;
	UpdateEyePosition();
}

void Camera::TurnRight()
{
	theta -= ROT_STEP;
	if (theta < 0.0f)
		theta += 360.0f;
	UpdateEyePosition();
}

void Camera::TurnUp() 
{
	if (phi + ROT_STEP < 180.0f)
	{
		phi += ROT_STEP;
		UpdateEyePosition();
	}
}

void Camera::TurnDown() 
{
	if (phi - ROT_STEP > 0.0f)
	{
		phi -= ROT_STEP;
		UpdateEyePosition();
	}
}

void Camera::GoForward()
{
	FOV /= zoom_factor;
	UpdateProjectionMatrix();
}

void Camera::GoBack()
{
	if (zoom_factor * FOV < 180.0f)
	{
		FOV *= zoom_factor;
		UpdateProjectionMatrix();
	}
}