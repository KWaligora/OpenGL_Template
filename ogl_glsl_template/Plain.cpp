#include <iostream>
#include "Plain.h"
#include "Texture.h"
#include "shaders.h"
#include <iostream>

Plain::Plain()
{
	SetShader("shaders/floor.vert", "shaders/floor.frag");
	SetBuffers();

	Texture tex;
	texture = tex.SetupTextures(L"textures/diffuse.png");

	modelMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	normal = glm::vec3(0.0f, 0.0f, -1.0f);
}

Plain::Plain(std::string vert, std::string frag)
{
	SetShader(vert, frag);
	SetBuffers();

	Texture tex;
	texture = tex.SetupTextures(L"textures/diffuse.png");

	modelMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	normal = glm::vec3(0.0f, 0.0f, -1.0f);


}

void Plain::SetShader(std::string vert, std::string frag)
{
	if (!setupShaders(vert, frag, PlainShaderProg))
		exit(3);

	colorLoc = glGetAttribLocation(PlainShaderProg, "vColor");
	texSamplerLoc = glGetAttribLocation(PlainShaderProg, "texSampler");
}
void Plain::SetBuffers()
{

	// wspolrzedne wierzcholkow pod³ogi
	float vertices[]
	{
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f,
	};

	// kolory wierzchokow pod³ogi
	float colors[] =
	{
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f
	};

	// wspolrzedne tekstury
	float uv[] =
	{
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbo);

	glBindVertexArray(vao); // dowiazanie pierwszego VAO    	

	// VBO dla wspolrzednych wierzcholkow
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // wlaczenie tablicy atrybutu wierzcholka - wspolrzedne
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0); // zdefiniowanie danych tablicy atrybutu wierzchoka - wspolrzedne

	// VBO dla kolorow
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(colorLoc); // wlaczenie tablicy atrybutu wierzcholka - kolory
	glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0); // zdefiniowanie danych tablicy atrybutu wierzcholka - kolory

	// VBO dla uv
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2); // wlaczenie tablicy atrybutu wierzcholka - wspolrzedne
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); // zdefiniowanie danych tablicy atrybutu wierzchoka - wspolrzedne

	glBindVertexArray(0);
}

void Plain::SetTexture(const wchar_t* filename)
{
	Texture tex;
	texture = tex.SetupTextures(filename);
}
void Plain::SetScale(glm::vec3 scale)
{
	modelMatrix = glm::scale(modelMatrix, scale);
}
void Plain::SetRotation(float rotation, glm::vec3 axis)
{
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), axis);

	/*rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation), axis);
	normal = glm::normalize(rotationMatrix * glm::vec4(normal, 1.0f));*/

}
void Plain::SetTranslation(glm::vec3 translation)
{
	modelMatrix = glm::translate(modelMatrix, translation);
	position += translation;
}

void Plain::Render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	glUseProgram(PlainShaderProg);

	GLint ModelLoc = glGetUniformLocation(PlainShaderProg, "model");
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	//Macierze widoku i projekcji
	GLuint projLoc = glGetUniformLocation(PlainShaderProg, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	GLuint viewLoc = glGetUniformLocation(PlainShaderProg, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	//tekstury
	texSamplerLoc = glGetUniformLocation(PlainShaderProg, "texSampler");
	glUniform1i(texSamplerLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//rysuj
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	
}

void Plain::Bilbording(glm::vec3 camPos)
{
	glm::vec3 newNormal = camPos - position;

	newNormal = glm::normalize(newNormal);
	if (newNormal != normal)
	{
		float angle = glm::acos(glm::dot(normal, newNormal));

		if (newNormal.x - normal.x > 0)
			angle *= -1;
		if (camPos.z > position.z)
			angle *= -1;
		if (!isnan(angle))
		{
			modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
			normal = newNormal;
		}
	}
}

void Plain::PrintVector(glm::vec3 vector)
{
	std::cout << "Current Position: " << vector.x << " " << vector.y << " " << vector.z<<std::endl;
}

Plain::~Plain()
{
	glDeleteProgram(PlainShaderProg); // usuniecie programu cieniowania
	glDeleteBuffers(3, vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteTextures(1, &texture);
}