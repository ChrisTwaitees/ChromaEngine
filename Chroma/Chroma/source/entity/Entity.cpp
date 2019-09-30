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
