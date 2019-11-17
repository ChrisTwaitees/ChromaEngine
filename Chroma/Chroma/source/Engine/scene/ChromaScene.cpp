#include "ChromaScene.h"

void ChromaScene::addEntity(IChromaEntity*& newEntity)
{
	newEntity->bindParentScene(this);
	m_entities.push_back(newEntity);
}

void ChromaScene::removeEntity(IChromaEntity& removeEntity)
{
}

void ChromaScene::removeLight(Light& removeLight)
{
}

void ChromaScene::setLights(std::vector<std::shared_ptr<Light>>& newLights)
{
	m_lights = newLights;
	for (std::shared_ptr<Light> light : newLights)
		if (light->type == Light::SUNLIGHT)
			m_sunLight = light.get();
}

void ChromaScene::setEntities(std::vector<IChromaEntity*>& newEntities)
{
	for (IChromaEntity* entity : newEntities)
		addEntity(entity);
}


ChromaScene::ChromaScene()
{
}


ChromaScene::~ChromaScene()
{
}
