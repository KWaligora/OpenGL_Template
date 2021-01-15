#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>

class Camera
{
public:
	Camera(int WindowWidth = 600, int windowHeight = 600);

	void SetViewMatrix();
	void SetViewMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 up);

	void SetProjectionMatrix();
	void SetProjectionMatrix(float fov, float aspecRation, float near, float far);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	
	void UpdateProjectionMatrix();

	void TurnLeft();
	void TurnRight();
	void TurnUp();
	void TurnDown();
	void GoForward();
	void GoBack();
	void GoUp();
	void GoDown();

private:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	float radius;
	float aspecRation;
	float FOV;
	float near;
	float far;
	float theta;
	float phi;
	float ROT_STEP;
	float zoom_factor;

	glm::vec3 eye; // pozycja oka w ukladzie swiata
	glm::vec3 center; // punkt na ktory patrzymy
	glm::vec3 up; // wektor do gory

	void UpdateEyePosition();
};

