#include "ChromaSceneManager.h"
#include <entity/ChromaEntity.h>

void ChromaSceneManager::addEntity(IChromaEntity*& newEntity)
{
	newEntity->bindParentScene(this);
	m_entities.push_back(newEntity);
}

void ChromaSceneManager::removeEntity(IChromaEntity& removeEntity)
{
}

void ChromaSceneManager::removeLight(Light& removeLight)
{
}

void ChromaSceneManager::setLights(std::vector<std::shared_ptr<Light>>& newLights)
{
	m_lights = newLights;
	for (std::shared_ptr<Light> light : newLights)
		if (light->type == Light::SUNLIGHT)
			m_sunLight = light.get();
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
