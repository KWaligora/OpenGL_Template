#pragma once
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>

class Plain
{
public:
	Plain();
	Plain(std::string vert, std::string frag);

	void Render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	void SetTexture(const wchar_t* filename);
	void SetScale(glm::vec3 scale);
	void SetRotation(float rotation, glm::vec3 axis);
	void SetTranslation(glm::vec3 translation);
	~Plain();
	

private:
	GLuint PlainShaderProg; // identyfikator programu cieniowania
	GLuint colorLoc; // lokalizacja atrybutu wierzcholka - kolor
	GLuint texSamplerLoc; // lokalizacja samplera

	GLuint vao;
	GLuint vbo[3];

	GLuint texture;

	glm::mat4 modelMatrix;

	void SetShader(std::string vert, std::string frag);
	void SetBuffers();
};

