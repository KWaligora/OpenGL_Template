#pragma once
#include "Camera.h"
#include <GL/glew.h>
#include "GLFW/glfw3.h"

class Floor
{
public:
	Floor();

	void Render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	void SetTexture(const wchar_t* filename);
	~Floor();
	

private:
	GLuint FloorShaderProg; // identyfikator programu cieniowania
	GLuint colorLoc; // lokalizacja atrybutu wierzcholka - kolor
	GLuint texSamplerLoc; // lokalizacja samplera

	GLuint vao;
	GLuint vbo[3];

	GLuint texture;

	void SetShader();
	void SetBuffers();
};

