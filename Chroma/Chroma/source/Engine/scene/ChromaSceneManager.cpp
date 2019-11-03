#include "ChromaSceneManager.h"


void ChromaSceneManager::removeCamera(Camera& removeCamera)
{
}

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

ChromaSceneManager::ChromaSceneManager()
{
}


ChromaSceneManager::~ChromaSceneManager()
{
}
