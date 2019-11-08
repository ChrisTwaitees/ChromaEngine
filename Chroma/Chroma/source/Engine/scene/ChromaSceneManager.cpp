#include "ChromaSceneManager.h"


void ChromaSceneManager::removeEntity(ChromaEntity& removeEntity)
{
}

void ChromaSceneManager::removeLight(Light& removeLight)
{
}

void ChromaSceneManager::setLights(std::vector<std::shared_ptr<Light>>& newLights)
{
	Lights = newLights;
	for (std::shared_ptr<Light> light : newLights)
	{
		if (light->type == Light::SUNLIGHT)
			SunLight = light.get();
	}
}

ChromaEntity* ChromaSceneManager::addNewEntity(IChromaComponent* component)
{
	// create new entity
	ChromaEntity* newEntity = new ChromaEntity;
	newEntity->addComponent(component);
	addEntity(newEntity);
	
	return newEntity;
}

ChromaSceneManager::ChromaSceneManager()
{
}


ChromaSceneManager::~ChromaSceneManager()
{
}
