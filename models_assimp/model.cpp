#include "model.h"
#include <iostream>
#include <fstream>
#include "shaders.h"
#include "Texture.h"

Model::Model(const std::string& filename, GLint vertexLoc, GLint normalLoc)
{
	Assimp::Importer importer;
	const aiScene* scene = nullptr;

	if (ImportModelFromFile(filename, importer, &scene))
		ModelFromScene(scene, vertexLoc, normalLoc);
}

void Model::Draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	glUseProgram(shaderProg);

	// macierz modelu
	GLint ModelLoc = glGetUniformLocation(shaderProg, "model");
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// macierze widoku i projekcji
	GLuint projLoc = glGetUniformLocation(shaderProg, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	GLuint viewLoc = glGetUniformLocation(shaderProg, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	//tekstury
	texSamplerLoc = glGetUniformLocation(shaderProg, "texSampler");
	glUniform1i(texSamplerLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	for( unsigned int i = 0; i < meshes.size(); ++i )
		meshes[i]->Draw();

	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec3 Model::GetBBmin() const
{
	return bbMin;
}

glm::vec3 Model::GetBBmax() const
{
	return bbMax;
}

glm::vec3 Model::GetCentroid() const
{
	return centroid;
}

void Model::SetShader(std::string vert, std::string frag)
{
	if (!setupShaders(vert, frag, shaderProg))
		exit(3);

	colorLoc = glGetAttribLocation(shaderProg, "vColor");
	texSamplerLoc = glGetAttribLocation(shaderProg, "texSampler");
}

void Model::SetTexture(const wchar_t* filename)
{
	Texture tex;
	texture = tex.SetupTextures(filename);
}

void Model::SetScale(glm::vec3 scale)
{
	modelMatrix = glm::scale(modelMatrix, scale);
}

void Model::SetRotation(float rotation, glm::vec3 axis)
{
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), axis);
}

void Model::SetTranslation(glm::vec3 translation)
{
	modelMatrix = glm::translate(modelMatrix, translation);
}

bool Model::ImportModelFromFile(const std::string& filename, Assimp::Importer& importer, const aiScene** scene)
{
	std::ifstream fin(filename.c_str());

	if (!fin.fail())
		fin.close();
	else
	{
		std::cerr << "Nie moge otworzyc pliku: " << filename.c_str() << std::endl;
		std::cerr << importer.GetErrorString() << std::endl;
		return false;
	}

	*scene = importer.ReadFile(filename, 0 | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality | aiProcess_GenSmoothNormals);
	//*scene = importer.ReadFile( filename, 0 | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality | aiProcess_GenNormals );

	if (!*scene)
	{
		std::cerr << importer.GetErrorString() << std::endl;
		return false;
	}

	return true;
}

void Model::ModelFromScene(const aiScene* scene, GLint vertexLoc, GLint normalLoc)
{
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
		meshes.push_back(new Mesh(scene->mMeshes[i], vertexLoc, normalLoc));

	ComputeBoundingBox();
	ComputeCentroid(scene);
}

void Model::ComputeBoundingBox()
{
	if (meshes.size() == 0)
		return;

	bbMin = meshes[0]->GetBBmin();
	bbMax = meshes[0]->GetBBmax();

	for (unsigned int i = 1; i < meshes.size(); ++i)
	{
		glm::vec3 bbmin = meshes[i]->GetBBmin();
		glm::vec3 bbmax = meshes[i]->GetBBmax();

		if (bbmin.x < bbMin.x)
			bbMin.x = bbmin.x;
		if (bbmin.y < bbMin.y)
			bbMin.y = bbmin.y;
		if (bbmin.z < bbMin.z)
			bbMin.z = bbmin.z;

		if (bbmax.x > bbMax.x)
			bbMax.x = bbmax.x;
		if (bbmax.y > bbMax.y)
			bbMax.y = bbmax.y;
		if (bbmax.z > bbMax.z)
			bbMax.z = bbmax.z;
	}
}

void Model::ComputeCentroid(const aiScene* scene)
{
	glm::vec3 c = glm::vec3(0.0f, 0.0f, 0.0f);
	unsigned int n = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		const aiMesh* mesh = scene->mMeshes[i];

		for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
		{
			const aiVector3D* vertex = &mesh->mVertices[j];

			c.x += vertex->x;
			c.y += vertex->y;
			c.z += vertex->z;

			++n;
		}
	}

	centroid = (n != 0) ? c * (1.0f / n) : c;
}