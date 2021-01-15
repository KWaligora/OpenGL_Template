#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>

class Camera
{
public:
	Camera(int WindowWidth = 600, int windowHeight = 600);

	void SetViewMatrix();

	void SetProjectionMatrix();

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void GoForward(float deltaTime);
	void GoBack(float deltaTime);
	void GoLeft(float deltaTime);
	void GoRight(float deltaTime);

private:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 rotationMatrix;

	float radius;
	float aspecRation;
	float FOV;
	float near;
	float far;
	float theta;
	float phi;
	float cameraSpeed;
	float zoom_factor;

	glm::vec3 eye; // pozycja oka w ukladzie swiata
	glm::vec3 center; // punkt na ktory patrzymy
	glm::vec3 up; // wektor do gory
};

