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
	m_Entities.insert(newEntity);
}

void Scene::AddAnimatedEntity(IEntity* const& newAnimatedEntity)
{
	// process
	ProcessNewEntity(newAnimatedEntity);

	// add to animated entities
	m_AnimatedEntities.insert(newAnimatedEntity);
}

void Scene::AddTransparentEntity(IEntity* const& newTransparentEntity)
{
	// process
	ProcessNewEntity(newTransparentEntity);

	// add to Transparent Entities
	m_AnimatedEntities.insert(newTransparentEntity);
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
