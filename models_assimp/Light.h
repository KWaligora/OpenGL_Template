#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

class Light
{
public:
	Light();

	void SetLightPos(glm::vec3 position);

	void SendData(GLuint shaderProgram, glm::mat4 modelMatrix, glm::mat4 viewMatrix);

	glm::vec4 lightPosition;
	glm::vec3 lightAmbient;
	glm::vec3 lightDiffuse;
	glm::vec3 lightSpecular;
};

