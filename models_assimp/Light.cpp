#include "Light.h"

Light::Light()
{
	lightPosition = glm::vec4(0.0f, 20.0f, -10.0f, 1.0f);

	lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	lightDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	lightSpecular = glm::vec3(1.0, 1.0, 1.0);
}

void Light::SetLightPos(glm::vec3 postion)
{
	lightPosition = glm::vec4(postion, 1.0f);
}

void Light::SetAmbient(glm::vec3 ambient)
{
	lightAmbient = ambient;
}

void Light::SetDiffuse(glm::vec3 diffuse)
{
	lightDiffuse = diffuse;
}

void Light::SetSpecular(glm::vec3 specular)
{
	lightSpecular = specular;
}

// wyslanie informacji o swietle do shadera
void Light::SendData(std::string fragmentShader, glm::mat4 modelMatrix, glm::mat4 viewMatrix)
{
	glm::mat4 mvMatrix = viewMatrix * modelMatrix;
	lightPosition = mvMatrix * lightPosition; // przejscie do wspolrzednych oka


}