#include "Scene.h"
#include <model/Model.h>
#include <render/Render.h>

namespace Chroma
{
	// Scene Contents
	// Render
	Camera*             Scene::m_RenderCamera;
	Light*              Scene::m_SunLight;
	SkyBox*             Scene::m_Skybox;
	IBL*				Scene::m_IBL;

	// Entities Components
	std::map<UID, IEntity*>    Scene::m_Entities;
	std::map<UID, IComponent*> Scene::m_Components;

	// UIDs
	//entities
	std::set<UID> Scene::m_EntityUIDs;
	std::set<UID> Scene::m_TransparentEntityUIDs;
	std::set<UID> Scene::m_AnimatedEntityUIDs;
	std::set<UID> Scene::m_IKComponentUIDs;

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

	std::set<UID> Scene::m_UIComponentUIDs;

	std::set<UID> Scene::m_StateMachineUIDs;

	std::set<UID> Scene::m_LightUIDs;

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
		if (m_Entities.find(UID) != m_Entities.end()) {
			return m_Entities.find(UID)->second;
		}
		else {
			return nullptr;
		}
	}

	void Scene::RemoveEntity(UID const& UID)
	{
		m_AnimatedEntityUIDs.erase(UID);
		m_TransparentEntityUIDs.erase(UID);
		m_EntityUIDs.erase(UID);

		// root entity list
		m_Entities.erase(UID);
	}

	IComponent* Scene::GetComponent(UID const& UID)
	{
		if (m_Components.find(UID) != m_Components.end()) {
			return m_Components.find(UID)->second;
		}
		else {
			return nullptr;
		}
	}

	void Scene::RemoveComponent(UID const& UID)
	{
		m_ComponentUIDs.erase(UID);

		m_AnimationComponentUIDs.erase(UID);
		m_AnimatedEntityUIDs.erase(UID);

		m_PhysicsComponentUIDs.erase(UID);

		m_CharacterControllerUIDs.erase(UID);

		m_StateMachineUIDs.erase(UID);

		m_PhysicsComponentUIDs.erase(UID);

		m_UIComponentUIDs.erase(UID);
		// render flags
		m_MeshComponentUIDs.erase(UID);
		m_SkinnedMeshComponentUIDs.erase(UID);
		m_ShadowCastingComponentUIDs.erase(UID);
		m_TransparentComponentUIDs.erase(UID);
		m_RenderableComponentUIDs.erase(UID);
		m_ForwardLitComponentUIDs.erase(UID);
		m_LitComponentUIDs.erase(UID);
		m_UnLitComponentUIDs.erase(UID);

		// lights
		m_LightUIDs.erase(UID);

		// root component list
		m_Components.erase(UID);
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

	void Scene::LoadIBL(std::string const& sourcePath)
	{
		m_IBL->LoadIBL(sourcePath); // image based lighting
		m_Skybox->setCubeMapID(m_IBL->GetEnvCubeMapID());

	}

	void Scene::Init()
	{
		// init members
		m_RenderCamera = new Camera();
		m_SunLight = new Light(Light::SUNLIGHT, glm::vec3(0.2, -0.8, 0.0), 1.0f);
		m_Skybox = new SkyBox();
		m_IBL =  new IBL(); // image based lighting

		// setting skybox to IBL environment map
		m_Skybox->setColorSpace(HDR);
		m_Skybox->setCubeMapID(m_IBL->GetEnvCubeMapID());
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

		// shadowmap attachment
		Chroma::Render::BindShadowMaps();

		// Debug
		CHROMA_INFO_UNDERLINE;
		CHROMA_INFO("CHROMA SCENE:: Scene Successfully Loaded.");
		// timing
		m_SceneBuildEndTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(m_SceneBuildEndTime - m_SceneBuildStartTime);
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
		m_Entities.erase(RemoveEntity.GetUID());
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

	void Scene::AddUIComponent(IComponent* const& newUIComponent)
	{
		// add to global component UIDs
		m_ComponentUIDs.insert(newUIComponent->GetUID());

		// collect component UID
		m_UIComponentUIDs.insert(newUIComponent->GetUID());

		// add component
		m_Components[newUIComponent->GetUID()] = newUIComponent;
	}

	void Scene::AddStateMachineComponent(IComponent* const& newStateMachineComponent)
	{
		// add to global component UIDs
		m_ComponentUIDs.insert(newStateMachineComponent->GetUID());

		// collect component UID
		m_StateMachineUIDs.insert(newStateMachineComponent->GetUID());

		// add component
		m_Components[newStateMachineComponent->GetUID()] = newStateMachineComponent;
	}

	void Scene::AddIKComponent(IComponent* const& newIKComponent)
	{
		// add to global component UIDs
		m_ComponentUIDs.insert(newIKComponent->GetUID());

		// collect component UID
		m_IKComponentUIDs.insert(newIKComponent->GetUID());

		// add component
		m_Components[newIKComponent->GetUID()] = newIKComponent;
	}

	void Scene::AddLight(IComponent* const& newLight)
	{
		// add to global component UIDs
		m_ComponentUIDs.insert(newLight->GetUID());

		// collect component UID
		m_LightUIDs.insert(newLight->GetUID());

		// add component
		m_Components[newLight->GetUID()] = newLight;
	}

	void Scene::RemoveLight(Light& RemoveLight)
	{
	
	}

	void Scene::SetLights(std::vector<Light*> newLights)
	{
		for (Light*& light : newLights)
		{
			if (light->type == Light::SUNLIGHT)
			{
				m_SunLight = light;
				AddLight(light);
			}
			else
				AddLight(light);

		}
			
	}

	void Scene::SetEntities(std::vector<IEntity*> const& newEntities)
	{
		for (IEntity* entity : newEntities)
			AddEntity(entity);
	}
}

