#include "ChromaEntity.h"


void ChromaEntity::bindCamera(Camera* newCamera)
{
	pCamera = newCamera;
}

void ChromaEntity::bindLights(std::vector<Light>* newLights)
{
	pLights = newLights;
}

void ChromaEntity::bindTexture(Texture newTexture)
{
	// to be implemented by inherited class
	std::cout << "ChromaEntity bind Texture" << std::endl;
}

void ChromaEntity::bindTextures(std::vector<Texture> textures_val)
{
	// to be implemented by inherited class
	std::cout << "ChromaEntity binding Textures" << std::endl;
}

ChromaEntity::ChromaEntity()
{
}


ChromaEntity::~ChromaEntity()
{
}

void ChromaEntity::Draw(Shader& shader)
{
	// to be implemented by inherited class
	std::cout << "ChromaEntity Draw with Shader Called" << std::endl;
}

void ChromaEntity::Draw()
{
	// to be implemented by inherited class
	std::cout << "ChromaEntity Draw Called" << std::endl;
}

void ChromaEntity::scale(glm::vec3 scalefactor)
{
	modelMat = glm::scale(modelMat, scalefactor);
}

void ChromaEntity::translate(glm::vec3 translatefactor)
{
	modelMat = glm::translate(modelMat, translatefactor);
}

void ChromaEntity::rotate(float degrees, glm::vec3 rotationaxis)
{
	modelMat = glm::rotate(modelMat, glm::radians(degrees), rotationaxis);
}


void ChromaEntity::setScale(glm::vec3 newscale)
{
	modelMat = glm::scale(identityMat, newscale);
}

void ChromaEntity::setPosition(glm::vec3 newposition)
{
	modelMat = glm::translate(identityMat, newposition);
}

