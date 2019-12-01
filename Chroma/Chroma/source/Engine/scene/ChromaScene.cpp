#include "ChromaScene.h"

glm::vec3 ChromaScene::calcAmbientLightColor()
{
	return m_sunLight->getDiffuse() * m_sunLight->getIntensity()* glm::vec3(.1);
}

void ChromaScene::initialize()
{
	m_skybox->setColorSpace(HDR);
	//m_skybox->setCubeMapID(testIBL.getEnvCubeMapID());
}

void ChromaScene::addEntity(IChromaEntity* const& newEntity)
{
	// bind parent scene
	newEntity->bindParentScene(this);
	// collect entity
	m_entities.push_back(newEntity);
	// check if entity has any transparent components break if found one
	for (IChromaComponent* meshComponent : newEntity->getMeshComponents())
	{
		if (((ChromaMeshComponent*)meshComponent)->isTransparent)
		{
			m_transparentEntities.push_back(newEntity);
			break;
		}
	}
}

void ChromaScene::removeEntity(IChromaEntity& removeEntity)
{
}

void ChromaScene::removeLight(Light& removeLight)
{
}

void ChromaScene::setLights(std::vector<Light*> newLights)
{
	m_lights = newLights;
	for (Light* light : newLights)
		if (light->type == Light::SUNLIGHT)
			m_sunLight = light;
}

void ChromaScene::setEntities(std::vector<IChromaEntity*> const& newEntities)
{
	for (IChromaEntity* entity : newEntities)
		addEntity(entity);
}


ChromaScene::ChromaScene()
{
	initialize();
}


ChromaScene::~ChromaScene()
{
}
