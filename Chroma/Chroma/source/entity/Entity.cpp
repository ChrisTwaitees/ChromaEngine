#include "Entity.h"


void Entity::bindCamera(Camera* newCamera)
{
	pCamera = newCamera;
}

void Entity::bindLights(std::vector<Light>* newLights)
{
	pLights = newLights;
}

void Entity::bindTexture(Texture newTexture)
{
	// to be implemented by inherited class
	std::cout << "Entity bind Texture" << std::endl;
}

void Entity::bindTextures(std::vector<Texture> textures_val)
{
	// to be implemented by inherited class
	std::cout << "Entity binding Textures" << std::endl;
}

Entity::Entity()
{
}


Entity::~Entity()
{
}

void Entity::Draw(Shader& shader)
{
	// to be implemented by inherited class
	std::cout << "Entity Draw with Shader Called" << std::endl;
}

void Entity::Draw()
{
	// to be implemented by inherited class
	std::cout << "Entity Draw Called" << std::endl;
}

void Entity::scale(glm::vec3 scalefactor)
{
	modelMat = glm::scale(modelMat, scalefactor);
}

void Entity::translate(glm::vec3 translatefactor)
{
	modelMat = glm::translate(modelMat, translatefactor);
}

void Entity::rotate(float degrees, glm::vec3 rotationaxis)
{
	modelMat = glm::rotate(modelMat, glm::radians(degrees), rotationaxis);
}


void Entity::setScale(glm::vec3 newscale)
{
	modelMat = glm::scale(identityMat, newscale);
}

void Entity::setPosition(glm::vec3 newposition)
{
	modelMat = glm::translate(identityMat, newposition);
}

