#include "model.h"
#include <iostream>
#include <fstream>
#include "shaders.h"
#include "Texture.h"

Model::Model()
{
	bbMin = glm::vec3( 0.0 );
	bbMax = glm::vec3( 0.0 );
	centroid = glm::vec3( 0.0 );

	modelMatrix = glm::mat4(1.0f);
}

Model::Model(const std::string& filename, GLint vertexLoc, GLint normalLoc)
{
	Assimp::Importer importer;
	const aiScene* scene = nullptr;

	if( importModelFromFile( filename, importer, &scene ) )
		modelFromScene( scene, vertexLoc, normalLoc );

	modelMatrix = glm::mat4(1.0f);
}

Model::Model(const aiScene *scene, GLint vertexLoc, GLint normalLoc)
{
	modelFromScene( scene, vertexLoc, normalLoc );
	modelMatrix = glm::mat4(1.0f);
}

Model::~Model()
{
	glDeleteTextures(1, &texture);
	for( unsigned int i = 0; i < meshes.size(); ++i )
		delete meshes[i];
}

void Model::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, Light light)
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

	SetLight(light, viewMatrix);

	// tekstury
	texSamplerLoc = glGetUniformLocation(shaderProg, "texSampler");
	glUniform1i(texSamplerLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	for( unsigned int i = 0; i < meshes.size(); ++i )
		meshes[i]->draw();

	glBindTexture(GL_TEXTURE_2D, 0);
}

Mesh* Model::getMesh(unsigned int n) const
{
	return ( n < meshes.size() ) ? meshes[n] : nullptr;
}

unsigned int Model::getNumberOfMeshes() const
{
	return meshes.size();
}

glm::vec3 Model::getBBmin() const
{
	return bbMin;
}

glm::vec3 Model::getBBmax() const
{
	return bbMax;
}

glm::vec3 Model::getCentroid() const
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

// przesyla informacje o oswietleniu i materiale do shadera
void Model::SetLight(Light light, glm::mat4 viewMatrix)
{
	glm::mat4 mvMatrix = viewMatrix * modelMatrix; //macierz modelu-widoku
	glm::vec4 lightPosition = mvMatrix * light.lightPosition; // przejscie do wspolrzednych oka

	GLuint lightPositionLoc = glGetUniformLocation(shaderProg, "lightPosition");
	GLuint lightAmbientLoc = glGetUniformLocation(shaderProg, "lightAmbient");
	GLuint lightDiffuseLoc = glGetUniformLocation(shaderProg, "lightDiffuse");
	GLuint lightSpecularLoc = glGetUniformLocation(shaderProg, "lightSpecular");

	glUniform4fv(lightPositionLoc, 1, glm::value_ptr(lightPosition));
	glUniform3fv(lightAmbientLoc, 1, glm::value_ptr(light.lightAmbient));
	glUniform3fv(lightDiffuseLoc, 1, glm::value_ptr(light.lightDiffuse));
	glUniform3fv(lightSpecularLoc, 1, glm::value_ptr(light.lightSpecular));

	GLuint materialAmbientLoc = glGetUniformLocation(shaderProg, "materialAmbient");
	GLuint materialDiffuseLoc = glGetUniformLocation(shaderProg, "materialDiffuse");
	GLuint materialSpecularLoc = glGetUniformLocation(shaderProg, "materialSpecular");
	GLuint materialShininessLoc = glGetUniformLocation(shaderProg, "materialShininess");

	glUniform3fv(materialDiffuseLoc, 1, glm::value_ptr(materialDiffuse));
	glUniform3fv(materialAmbientLoc, 1, glm::value_ptr(materialAmbient));
	glUniform3fv(materialSpecularLoc, 1, glm::value_ptr(materialSpecular));
	glUniform1f(materialShininessLoc, shininess);

	GLuint normalMatrixLoc = glGetUniformLocation(shaderProg, "normalMatrix");
	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(mvMatrix));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

bool Model::importModelFromFile( const std::string& filename, Assimp::Importer& importer, const aiScene **scene )
{
	std::ifstream fin( filename.c_str() );

	if( !fin.fail() )
		fin.close();
	else
	{
		std::cerr << "Nie moge otworzyc pliku: " << filename.c_str() << std::endl;
		std::cerr << importer.GetErrorString() << std::endl;
		return false;
	}

	*scene = importer.ReadFile( filename, 0 | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality | aiProcess_GenSmoothNormals );
	//*scene = importer.ReadFile( filename, 0 | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality | aiProcess_GenNormals );

	if( !*scene )
	{
		std::cerr << importer.GetErrorString() << std::endl;
		return false;
	}

	return true;
}

void Model::modelFromScene(const aiScene *scene, GLint vertexLoc, GLint normalLoc)
{
	for( unsigned int i = 0; i < scene->mNumMeshes; ++i )
		meshes.push_back( new Mesh( scene->mMeshes[i], vertexLoc, normalLoc ) );

	computeBoundingBox();
	computeCentroid( scene );
}

void Model::computeBoundingBox()
{
	if( meshes.size() == 0 )
		return;

	bbMin = meshes[0]->getBBmin();
	bbMax = meshes[0]->getBBmax();

	for( unsigned int i = 1; i < meshes.size(); ++i )
	{
		glm::vec3 bbmin = meshes[i]->getBBmin();
		glm::vec3 bbmax = meshes[i]->getBBmax();

		if( bbmin.x < bbMin.x )
			bbMin.x = bbmin.x;
		if( bbmin.y < bbMin.y )
			bbMin.y = bbmin.y;
		if( bbmin.z < bbMin.z )
			bbMin.z = bbmin.z;

		if( bbmax.x > bbMax.x )
			bbMax.x = bbmax.x;
		if( bbmax.y > bbMax.y )
			bbMax.y = bbmax.y;
		if( bbmax.z > bbMax.z )
			bbMax.z = bbmax.z;
	}
}

void Model::computeCentroid(const aiScene *scene)
{
	glm::vec3 c = glm::vec3( 0.0f, 0.0f, 0.0f );
	unsigned int n = 0;
	for( unsigned int i = 0; i < scene->mNumMeshes; ++i )
	{
		const aiMesh *mesh = scene->mMeshes[i];

		for( unsigned int j = 0; j < mesh->mNumVertices; ++j )
		{
			const aiVector3D *vertex = &mesh->mVertices[j];

			c.x += vertex->x;
			c.y += vertex->y;
			c.z += vertex->z;

			++n;
		}
	}

	centroid = ( n != 0 ) ? c * (1.0f / n) : c;
}
