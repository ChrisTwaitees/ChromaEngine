#include "ChromaScene.h"

glm::vec3 ChromaScene::CalculateAmbientLightColor()
{
	return m_SunLight->getDiffuse() * m_SunLight->getIntensity()* glm::vec3(.1);
}

void ChromaScene::Initialize()
{
	// setting skybox to IBL environment map
	m_Skybox->setColorSpace(HDR);
	m_Skybox->setCubeMapID(m_IBL->getEnvCubeMapID());
}

void ChromaScene::AddEntity(IChromaEntity* const& newEntity)
{
	// bind parent scene
	newEntity->bindParentScene(this);
	// collect entity
	m_Entities.push_back(newEntity);
	// check if entity has any transparent components break if found one
	for (IChromaComponent* meshComponent : newEntity->getMeshComponents())
	{
		if (((ChromaMeshComponent*)meshComponent)->m_IsTransparent)
		{
			m_TransparentEntities.push_back(newEntity);
			break;
		}
	}
}

void ChromaScene::RemoveEntity(IChromaEntity& RemoveEntity)
{
}

void ChromaScene::RemoveLight(Light& RemoveLight)
{
}

void ChromaScene::SetLights(std::vector<Light*> newLights)
{
	m_Lights = newLights;
	for (Light* light : newLights)
		if (light->type == Light::SUNLIGHT)
			m_SunLight = light;
}

void ChromaScene::SetEntities(std::vector<IChromaEntity*> const& newEntities)
{
	for (IChromaEntity* entity : newEntities)
		AddEntity(entity);
}


ChromaScene::ChromaScene()
{
	Initialize();
}


ChromaScene::~ChromaScene()
{
}
