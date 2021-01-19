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

public:
	Model();
	Model(const std::string& filename, GLint vertexLoc = 0, GLint normalLoc = 1);
	Model(const aiScene *scene, GLint vertexLoc = 0, GLint normalLoc = 1);
	~Model();

	void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	Mesh* getMesh(unsigned int n) const;
	unsigned int getNumberOfMeshes() const;
	glm::vec3 getBBmin() const;
	glm::vec3 getBBmax() const;
	glm::vec3 getCentroid() const;
	glm::mat4 GetModelMatrix();
	GLuint GetShaderProgram();
	void SetShader(std::string vert, std::string frag);
	void SetTexture(const wchar_t* filename);
	void SetScale(glm::vec3 scale);
	void SetRotation(float rotation, glm::vec3 axis);
	void SetTranslation(glm::vec3 translation);

private:
	bool importModelFromFile( const std::string& filename, Assimp::Importer& importer, const aiScene **scene );
	void modelFromScene(const aiScene *scene, GLint vertexLoc, GLint normalLoc);
	void computeBoundingBox();
	void computeCentroid(const aiScene *scene);
};

#endif /*__MODEL_H__*/