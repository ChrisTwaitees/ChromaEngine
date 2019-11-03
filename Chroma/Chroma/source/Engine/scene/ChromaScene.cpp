#include "ChromaScene.h"



void ChromaScene::removeCamera(Camera& removeCamera)
{
}

void ChromaScene::removeEntity(ChromaEntity& removeEntity)
{
}

void ChromaScene::removeLight(Light& removeLight)
{
}

void ChromaScene::setLights(std::vector<std::shared_ptr<Light>>& newLights)
{
	Lights = newLights;
	for (std::shared_ptr<Light> light : newLights)
	{
		if (light->type == Light::SUNLIGHT)
			SunLight = light.get();
	}
}

ChromaScene::ChromaScene()
{
}


ChromaScene::~ChromaScene()
{
}
