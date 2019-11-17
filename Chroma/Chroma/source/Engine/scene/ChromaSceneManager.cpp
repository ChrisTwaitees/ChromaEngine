#include "ChromaSceneManager.h"
#include <entity/ChromaEntity.h>

void ChromaSceneManager::removeEntity(IChromaEntity& removeEntity)
{
}

void ChromaSceneManager::removeLight(Light& removeLight)
{
}

void ChromaSceneManager::setLights(std::vector<std::shared_ptr<Light>>& newLights)
{
	Lights = newLights;
	for (std::shared_ptr<Light> light : newLights)
		if (light->type == Light::SUNLIGHT)
			SunLight = light.get();
}

IChromaEntity* ChromaSceneManager::addNewEntity(IChromaComponent* component)
{
	// create new entity
	IChromaEntity* newEntity = new ChromaEntity();
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
