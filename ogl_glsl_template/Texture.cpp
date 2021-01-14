#include "Texture.h"
#include <IL/il.h>
#include <iostream>

/*------------------------------------------------------------------------------------------
** funkcja ladujaca tekstury
**------------------------------------------------------------------------------------------*/
GLuint Texture::SetupTextures(const wchar_t* filename)
{
	glGenTextures(1, &texObj);
	glBindTexture(GL_TEXTURE_2D, texObj);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	LoadTexture(GL_TEXTURE_2D, filename);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texObj;
}

/*------------------------------------------------------------------------------------------
** funkcja ladujaca teksture z pliku
** tex - typ tekstury
** filename - nazwa pliku z tekstura do zaladowania
**------------------------------------------------------------------------------------------*/
void Texture::LoadTexture(GLuint tex, const wchar_t* filename)
{
	ilInit();

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	ILuint imageName;

	ilGenImages(1, &imageName);
	ilBindImage(imageName);

	if (!ilLoadImage(filename))
	{
		ILenum err = ilGetError();
		std::cerr << "Blad: " << err << std::endl;
		std::cerr << "      " << ilGetString(err) << std::endl;

		ilBindImage(0);
		ilDeleteImages(1, &imageName);

		exit(1);
	}

	glTexImage2D(tex, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());

	ilBindImage(0);
	ilDeleteImages(1, &imageName);
}
