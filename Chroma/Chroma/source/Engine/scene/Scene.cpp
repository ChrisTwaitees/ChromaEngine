#include "Scene.h"

glm::vec3 Scene::CalculateAmbientLightColor()
{
	return m_SunLight->getDiffuse() * m_SunLight->getIntensity()* glm::vec3(.1);
}

void Scene::Initialize()
{
	// setting skybox to IBL environment map
	m_Skybox->setColorSpace(HDR);
	m_Skybox->setCubeMapID(m_IBL->getEnvCubeMapID());
}

void Scene::AddEntity(IEntity* const& newEntity)
{
	// bind parent scene
	newEntity->bindParentScene(this);
	// collect entity
	m_Entities.push_back(newEntity);
	// check if entity has any transparent components break if found one
	for (IComponent* meshComponent : newEntity->getMeshComponents())
	{
		if (((MeshComponent*)meshComponent)->m_IsTransparent)
		{
			m_TransparentEntities.push_back(newEntity);
			break;
		}
	}
}

void Scene::RemoveEntity(IEntity& RemoveEntity)
{
}

void Scene::RemoveLight(Light& RemoveLight)
{
}

void Scene::SetLights(std::vector<Light*> newLights)
{
	m_Lights = newLights;
	for (Light* light : newLights)
		if (light->type == Light::SUNLIGHT)
			m_SunLight = light;
}

void Scene::SetEntities(std::vector<IEntity*> const& newEntities)
{
	for (IEntity* entity : newEntities)
		AddEntity(entity);
}


Scene::Scene()
{
	Initialize();
}


Scene::~Scene()
{
}
