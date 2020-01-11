#include "Scene.h"


namespace Chroma
{
	// Scene Contents
	// Render
	Camera*             Scene::m_RenderCamera;
	Light*              Scene::m_SunLight;
	SkyBox*             Scene::m_Skybox;
	std::vector<Light*> Scene::m_Lights;
	IBL*				Scene::m_IBL;

	// Entities Components
	std::map<UID, IEntity*>    Scene::m_Entities;
	std::map<UID, IComponent*> Scene::m_Components;

	// UIDs
	//entities
	std::set<UID> Scene::m_EntityUIDs;
	std::set<UID> Scene::m_TransparentEntityUIDs;
	std::set<UID> Scene::m_AnimatedEntityUIDs;

	//components
	std::set<UID> Scene::m_ComponentUIDs;

	std::set<UID> Scene::m_MeshComponentUIDs;
	std::set<UID> Scene::m_SkinnedMeshComponentUIDs;

	std::set<UID> Scene::m_AnimationComponentUIDs;
	std::set<UID> Scene::m_CharacterControllerUIDs;

	std::set<UID> Scene::m_RenderableComponentUIDs;
	std::set<UID> Scene::m_LitComponentUIDs;
	std::set<UID> Scene::m_ShadowCastingComponentUIDs;
	std::set<UID> Scene::m_TransparentComponentUIDs;
	std::set<UID> Scene::m_UnLitComponentUIDs;

	std::set<UID> Scene::m_PhysicsComponentUIDs;

	glm::vec3 Scene::CalculateAmbientLightColor()
	{
		return m_SunLight->getDiffuse() * m_SunLight->getIntensity() * glm::vec3(.5f);
	}

	IEntity* Scene::GetEntity(UID const& UID)
	{

		return m_Entities.find(UID)->second;
	}

	IComponent* Scene::GetComponent(UID const& UID)
	{
		return m_Components.find(UID)->second;
	}

	void Scene::Init()
	{
		// init members
		m_RenderCamera = new Camera();
		m_SunLight = new Light(Light::SUNLIGHT, glm::vec3(0.2, -0.8, 0.0), 1.0f);
		m_Skybox = new SkyBox(m_RenderCamera);
		m_IBL =  new IBL(); // image based lighting

		// setting skybox to IBL environment map
		m_Skybox->setColorSpace(HDR);
		m_Skybox->setCubeMapID(m_IBL->getEnvCubeMapID());
	}

	void Scene::PostSceneBuild()
	{
		// entities
		for (UID const& entityUID : m_EntityUIDs)
			GetEntity(entityUID)->Init();
		// components
		for (UID const& componentUID : m_ComponentUIDs)
			GetComponent(componentUID)->Init();
		// Debug
		CHROMA_INFO_UNDERLINE;
		CHROMA_INFO("CHROMA SCENE:: Scene Successfully Loaded.");
		CHROMA_INFO_UNDERLINE;
	}

	void Scene::AddEntity(IEntity* const& newEntity)
	{
		// collect UID
		m_EntityUIDs.insert(newEntity->GetUID());

		// collect entity
		m_Entities[newEntity->GetUID()] = newEntity;
	}

	void Scene::AddAnimatedEntity(IEntity* const& newAnimatedEntity)
	{
		// add UID to animated entity UIDs
		m_AnimatedEntityUIDs.insert(newAnimatedEntity->GetUID());
	}

	void Scene::AddTransparentEntity(IEntity* const& newTransparentEntity)
	{
		// add UID to transparent entity UIDs
		m_TransparentEntityUIDs.insert(newTransparentEntity->GetUID());
	}

	void Scene::RemoveEntity(IEntity& RemoveEntity)
	{
	}

	float Scene::GetEntityDistanceToCamera(UID const& UID)
	{
		return glm::length(GetEntity(UID)->GetTranslation() - m_RenderCamera->GetPosition());
	}


	void Scene::AddAnimationComponent(IComponent* const& newAnimationComponent)
	{
		// add to global component UIDs
		m_ComponentUIDs.insert(newAnimationComponent->GetUID());

		// collect component UID
		m_AnimationComponentUIDs.insert(newAnimationComponent->GetUID());

		// add component
		m_Components[newAnimationComponent->GetUID()] = newAnimationComponent;
	}

	void Scene::AddCharacterControllerComponent(IComponent* const& newCharacterControllerComponent)
	{
		// add to global component UIDs
		m_ComponentUIDs.insert(newCharacterControllerComponent->GetUID());

		// collect component UID
		m_CharacterControllerUIDs.insert(newCharacterControllerComponent->GetUID());

		// add component
		m_Components[newCharacterControllerComponent->GetUID()] = newCharacterControllerComponent;
	}

	void Scene::AddMeshComponent(IComponent* const& newMeshComponent)
	{
		// add to global component UIDs
		m_ComponentUIDs.insert(newMeshComponent->GetUID());

		// collect component UID
		m_MeshComponentUIDs.insert(newMeshComponent->GetUID());

		// add component
		m_Components[newMeshComponent->GetUID()] = newMeshComponent;

		// check for rendering features
		if (((MeshComponent*)newMeshComponent)->m_IsRenderable)
			m_RenderableComponentUIDs.insert(newMeshComponent->GetUID());
		if (((MeshComponent*)newMeshComponent)->m_IsLit && ((MeshComponent*)newMeshComponent)->m_IsTransparent == false)
			m_LitComponentUIDs.insert(newMeshComponent->GetUID());
		if (((MeshComponent*)newMeshComponent)->m_CastShadows)
			m_ShadowCastingComponentUIDs.insert(newMeshComponent->GetUID());
		if (((MeshComponent*)newMeshComponent)->m_IsTransparent)
			m_TransparentComponentUIDs.insert(newMeshComponent->GetUID());
		if (((MeshComponent*)newMeshComponent)->m_IsLit == false && ((MeshComponent*)newMeshComponent)->m_IsTransparent == false)
			m_UnLitComponentUIDs.insert(newMeshComponent->GetUID());
		if (((MeshComponent*)newMeshComponent)->m_IsTransparent)
			m_TransparentComponentUIDs.insert(newMeshComponent->GetUID());
		if (((MeshComponent*)newMeshComponent)->m_IsSkinned)
			m_SkinnedMeshComponentUIDs.insert(newMeshComponent->GetUID());
	}

	void Scene::AddPhysicsComponent(IComponent* const& newPhysicsComponent)
	{
		// add to global component UIDs
		m_ComponentUIDs.insert(newPhysicsComponent->GetUID());

		// collect component UID
		m_PhysicsComponentUIDs.insert(newPhysicsComponent->GetUID());

		// add component
		m_Components[newPhysicsComponent->GetUID()] = newPhysicsComponent;
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
}

