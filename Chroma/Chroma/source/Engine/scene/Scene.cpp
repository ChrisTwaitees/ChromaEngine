#include "Scene.h"

glm::vec3 Scene::CalculateAmbientLightColor()
{
	return m_SunLight->getDiffuse() * m_SunLight->getIntensity()* glm::vec3(.1);
}

IEntity* Scene::GetEntity(std::string UID)
{
	if (m_Entities.find(UID) != m_Entities.end())
	{
		return m_Entities.find(UID)->second;
	}
}


void Scene::Initialize()
{
	// setting skybox to IBL environment map
	m_Skybox->setColorSpace(HDR);
	m_Skybox->setCubeMapID(m_IBL->getEnvCubeMapID());
}

void Scene::ProcessNewEntity(IEntity* const& newEntity)
{
	// bind parent scene
	newEntity->bindParentScene(this);
}

void Scene::AddEntity(IEntity* const& newEntity)
{
	// process
	ProcessNewEntity(newEntity);

	// collect entity
	m_EntityUIDs.insert(newEntity->GetUID());

	// collect entity
	m_Entities[newEntity->GetUID()] = newEntity;
}

void Scene::AddAnimatedEntity(IEntity* const& newAnimatedEntity)
{
	// process
	ProcessNewEntity(newAnimatedEntity);

	// add UID to animated entity UIDs
	m_AnimatedEntityUIDs.insert(newAnimatedEntity->GetUID());
}

void Scene::AddTransparentEntity(IEntity* const& newTransparentEntity)
{
	// process
	ProcessNewEntity(newTransparentEntity);

	// add UID to transparent entity UIDs
	m_TransparentEntityUIDs.insert(newTransparentEntity->GetUID());
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
