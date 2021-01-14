#pragma once

#include <GLFW/glfw3.h>

class Texture
{
public:
	GLuint SetupTextures(const wchar_t* filename);

private:
	GLuint texObj;
	void LoadTexture(GLuint tex, const wchar_t* filename);
};

