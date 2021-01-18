#ifndef __MODEL_H__
#define __MODEL_H__

#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <glm\glm.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/PostProcess.h>
#include <assimp/Scene.h>

#include "mesh.h"

class Model
{
	std::vector<Mesh*> meshes;
	glm::vec3 bbMin;
	glm::vec3 bbMax;
	glm::vec3 centroid;

	glm::mat4 modelMatrix;

	GLuint shaderProg; // identyfikator programu cieniowania

	GLuint colorLoc; // lokalizacja atrybutu wierzcholka - kolor
	GLuint texSamplerLoc; // lokalizacja samplera
	GLuint texture;

	bool ImportModelFromFile(const std::string& filename, Assimp::Importer& importer, const aiScene** scene);
	void ModelFromScene(const aiScene* scene, GLint vertexLoc, GLint normalLoc);
	void ComputeBoundingBox();
	void ComputeCentroid(const aiScene* scene);

public:
	Model(const std::string& filename, GLint vertexLoc = 0, GLint normalLoc = 1);

	void Draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	glm::vec3 GetBBmin() const;
	glm::vec3 GetBBmax() const;
	glm::vec3 GetCentroid() const;
	void SetShader(std::string vert, std::string frag);
	void SetTexture(const wchar_t* filename);
	void SetScale(glm::vec3 scale);
	void SetRotation(float rotation, glm::vec3 axis);
	void SetTranslation(glm::vec3 translation);
};

#endif /*__MODEL_H__*/