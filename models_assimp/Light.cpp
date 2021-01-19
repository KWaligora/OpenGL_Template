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