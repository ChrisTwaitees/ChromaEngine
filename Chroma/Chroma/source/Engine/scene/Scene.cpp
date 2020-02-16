#include "Scene.h"
#include <model/Model.h>

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
	std::set<UID> Scene::m_ForwardLitComponentUIDs;

	std::set<UID> Scene::m_PhysicsComponentUIDs;

	// timing
	std::chrono::steady_clock::time_point Scene::m_SceneBuildStartTime;
	std::chrono::steady_clock::time_point Scene::m_SceneBuildEndTime;

	void Scene::ProcessMeshComponentRenderFlags(IComponent* const& newMeshComponent)
	{
		// check for rendering features
		// renderable
		if (((MeshComponent*)newMeshComponent)->GetIsRenderable())
			m_RenderableComponentUIDs.insert(newMeshComponent->GetUID());
		else
			SafeRemoveComponentUID(m_RenderableComponentUIDs, newMeshComponent->GetUID());
		// islit
		if (((MeshComponent*)newMeshComponent)->GetIsLit())
			m_LitComponentUIDs.insert(newMeshComponent->GetUID());
		else
			SafeRemoveComponentUID(m_LitComponentUIDs, newMeshComponent->GetUID());
		// unlit
		if (((MeshComponent*)newMeshComponent)->GetIsUnlit())
			m_UnLitComponentUIDs.insert(newMeshComponent->GetUID());
		else
			SafeRemoveComponentUID(m_UnLitComponentUIDs, newMeshComponent->GetUID());
		// casts shadows
		if (((MeshComponent*)newMeshComponent)->GetCastsShadows())
			m_ShadowCastingComponentUIDs.insert(newMeshComponent->GetUID());
		else
			SafeRemoveComponentUID(m_ShadowCastingComponentUIDs, newMeshComponent->GetUID());
		// transparent
		if (((MeshComponent*)newMeshComponent)->GetIsTransparent())
			m_TransparentComponentUIDs.insert(newMeshComponent->GetUID());
		else
			SafeRemoveComponentUID(m_TransparentComponentUIDs, newMeshComponent->GetUID());
		// is skinned
		if (((MeshComponent*)newMeshComponent)->GetIsSkinned())
			m_SkinnedMeshComponentUIDs.insert(newMeshComponent->GetUID());
		else
			SafeRemoveComponentUID(m_SkinnedMeshComponentUIDs, newMeshComponent->GetUID());
		// forward lit
		if (((MeshComponent*)newMeshComponent)->GetIsForwardLit())
			m_ForwardLitComponentUIDs.insert(newMeshComponent->GetUID());
		else
			SafeRemoveComponentUID(m_ForwardLitComponentUIDs, newMeshComponent->GetUID());
	}

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

	void Scene::SafeRemoveComponentUID(std::set<UID>& componentUIDList, UID const& removeUID)
	{
		// Search for element 
		std::set<UID>::iterator it = componentUIDList.find(removeUID);

		// Check if Iterator is valid
		if (it != componentUIDList.end())
		{
			// Deletes the element pointing by iterator it
			componentUIDList.erase(it);
		}
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

	void Scene::PreSceneBuild()
	{
		// Debug
		CHROMA_INFO_UNDERLINE;
		CHROMA_INFO("CHROMA SCENE:: Scene Loading...");
		CHROMA_INFO_UNDERLINE;

		// timing
		m_SceneBuildStartTime = std::chrono::high_resolution_clock::now();
	}

	void Scene::PostSceneBuild()
	{
		// entities
		for (UID const& entityUID : m_EntityUIDs)
			GetEntity(entityUID)->Init();
		// components
		for (UID const& componentUID : m_ComponentUIDs)
			GetComponent(componentUID)->Init();
		// timing
		m_SceneBuildEndTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(m_SceneBuildEndTime - m_SceneBuildStartTime);

		// Debug
		CHROMA_INFO_UNDERLINE;
		CHROMA_INFO("CHROMA SCENE:: Scene Successfully Loaded.");
		CHROMA_INFO("CHROMA SCENE:: Scene Load Took : {0} seconds", (float)duration.count()/1000.0f);
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

		// process renderflags
		if (Model* modelMeshComponent = dynamic_cast<Model*>(newMeshComponent))
		{
			for (UID const& uid : modelMeshComponent->GetMeshUIDs())
			{
				ProcessMeshComponentRenderFlags(GetComponent(uid));
			}
		}
		else
		{
			ProcessMeshComponentRenderFlags(newMeshComponent);
		}

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

