#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

class Light
{
public:
	glm::vec4 lightPosition;
	glm::vec3 lightAmbient;
	glm::vec3 lightDiffuse;
	glm::vec3 lightSpecular;

	Light();

	void SetLightPos(glm::vec3 position);	
};

