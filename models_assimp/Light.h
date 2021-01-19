#pragma once

#include <glm/glm.hpp>
#include <iostream>

class Light
{
public:
	Light();

	void SetLightPos(glm::vec3 position);

	void SetAmbient(glm::vec3 ambient);
	void SetDiffuse(glm::vec3 diffuse);
	void SetSpecular(glm::vec3 specular);

	void SendData(std::string fragmentShader, glm::mat4 modelMatrix, glm::mat4 viewMatrix);

private:
	glm::vec4 lightPosition;

	glm::vec3 lightAmbient;
	glm::vec3 lightDiffuse;
	glm::vec3 lightSpecular;
};

