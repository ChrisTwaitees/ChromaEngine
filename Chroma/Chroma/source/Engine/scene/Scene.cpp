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
	IBL* 				Scene::m_IBL;
	Texture				Scene::m_SceneNoise;

	// Entities Components
	std::map<UID, IEntity*>    Scene::m_Entities;
	std::map<UID, IComponent*> Scene::m_Components;

	// UIDs
	//entities
	std::set<UID> Scene::m_EntityUIDs;
	std::set<UID> Scene::m_AnimatedEntityUIDs;

	//components
	std::set<UID> Scene::m_ComponentUIDs;
	
	//components - animation
	std::set<UID> Scene::m_AnimationComponentUIDs;
	std::set<UID> Scene::m_CharacterControllerUIDs;
	std::set<UID> Scene::m_AnimConstraintComponentUIDs;

	//components - visual
	std::set<UID> Scene::m_MeshComponentUIDs;
	std::set<UID> Scene::m_SkinnedMeshComponentUIDs;
	std::set<UID> Scene::m_RenderableComponentUIDs;
	std::set<UID> Scene::m_LitComponentUIDs;
	std::set<UID> Scene::m_ShadowCastingComponentUIDs;
	std::set<UID> Scene::m_ShadowReceivingComponentUIDs;
	std::set<UID> Scene::m_TransparentComponentUIDs;
	std::set<UID> Scene::m_UnLitComponentUIDs;
	std::set<UID> Scene::m_ForwardLitComponentUIDs;

	std::set<UID> Scene::m_PhysicsComponentUIDs;

	std::set<UID> Scene::m_UIComponentUIDs;

	std::set<UID> Scene::m_StateMachineUIDs;

	// Lights
	std::set<UID> Scene::m_LightUIDs;
	bool Scene::m_LightsDirty{ false };

	// Scene State
	Scene::SceneState Scene::m_SceneState{ kSceneNotBuilt };

	// timing
	std::chrono::steady_clock::time_point Scene::m_SceneBuildStartTime;
	std::chrono::steady_clock::time_point Scene::m_SceneBuildEndTime;

	void Scene::ProcessMeshComponentRenderFlags(IComponent* const& newMeshComponent)
	{
		// check for rendering features
		// renderable
		if ( static_cast<MeshComponent*>(newMeshComponent)->GetIsRenderable())
			m_RenderableComponentUIDs.insert(newMeshComponent->GetUID());
		else
			SafeRemoveComponentUID(m_RenderableComponentUIDs, newMeshComponent->GetUID());
		// islit
		if (static_cast<MeshComponent*>(newMeshComponent)->GetIsLit())
			m_LitComponentUIDs.insert(newMeshComponent->GetUID());
		else
			SafeRemoveComponentUID(m_LitComponentUIDs, newMeshComponent->GetUID());
		// unlit
		if (static_cast<MeshComponent*>(newMeshComponent)->GetIsUnlit())
			m_UnLitComponentUIDs.insert(newMeshComponent->GetUID());
		else
			SafeRemoveComponentUID(m_UnLitComponentUIDs, newMeshComponent->GetUID());
		// casts shadows
		if (static_cast<MeshComponent*>(newMeshComponent)->GetCastsShadows())
			m_ShadowCastingComponentUIDs.insert(newMeshComponent->GetUID());
		else
			SafeRemoveComponentUID(m_ShadowCastingComponentUIDs, newMeshComponent->GetUID());
		// receives shadows
		if (static_cast<MeshComponent*>(newMeshComponent)->GetReceivesShadows())
			m_ShadowReceivingComponentUIDs.insert(newMeshComponent->GetUID());
		else
			SafeRemoveComponentUID(m_ShadowReceivingComponentUIDs, newMeshComponent->GetUID());
		// transparent
		if (static_cast<MeshComponent*>(newMeshComponent)->GetIsTransparent())
			m_TransparentComponentUIDs.insert(newMeshComponent->GetUID());
		else
			SafeRemoveComponentUID(m_TransparentComponentUIDs, newMeshComponent->GetUID());
		// is skinned
		if (static_cast<MeshComponent*>(newMeshComponent)->GetIsSkinned())
			m_SkinnedMeshComponentUIDs.insert(newMeshComponent->GetUID());
		else
			SafeRemoveComponentUID(m_SkinnedMeshComponentUIDs, newMeshComponent->GetUID());
		// forward lit
		if (static_cast<MeshComponent*>(newMeshComponent)->GetIsForwardLit())
			m_ForwardLitComponentUIDs.insert(newMeshComponent->GetUID());
		else
			SafeRemoveComponentUID(m_ForwardLitComponentUIDs, newMeshComponent->GetUID());
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
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsRemoving)
		
		// UIDs
		m_EntityUIDs.erase(UID);

		// root entity list
		m_Entities.erase(UID);

		// State 
		SCENE_RESETSTATE
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
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsRemoving)

		m_ComponentUIDs.erase(UID);

		m_AnimationComponentUIDs.erase(UID);

		m_AnimConstraintComponentUIDs.erase(UID);

		m_PhysicsComponentUIDs.erase(UID);

		m_CharacterControllerUIDs.erase(UID);

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

		// statemachine
		m_StateMachineUIDs.erase(UID);

		// lights
		m_LightUIDs.erase(UID);

		// root component list
		m_Components.erase(UID);

		// State 
		SCENE_RESETSTATE
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
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsAdding)

		m_IBL->LoadIBL(sourcePath); // image based lighting
		m_Skybox->setCubeMapID(m_IBL->GetEnvCubeMapID());

		// State 
		SCENE_RESETSTATE
	}

	void Scene::Init()
	{
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsInitializing)

		// init members
		m_RenderCamera = new Camera();
		m_SunLight = new Light(Chroma::Type::Light::kSunlight, glm::vec3(0.2, -0.8, 0.0), 1.0f);
		m_IBL = new IBL();
		m_Skybox = new SkyBox();
		m_SceneNoise = Texture("resources/textures/noise/noise_00.jpg");

		// setting skybox to IBL environment map
		m_Skybox->setColorSpace(HDR);
		m_Skybox->setCubeMapID(m_IBL->GetEnvCubeMapID());

		// State 
		SCENE_RESETSTATE
	}

	void Scene::Update()
	{
		m_LightsDirty = false;
	}

	void Scene::PreSceneBuild()
	{
		// Debug
		CHROMA_INFO_UNDERLINE;
		CHROMA_INFO("CHROMA SCENE:: Scene Loading...");
		CHROMA_INFO_UNDERLINE;

		// State
		m_SceneState = SceneState::kSceneNotBuilt;

		// timing
		m_SceneBuildStartTime = std::chrono::high_resolution_clock::now();
	}

	void Scene::PostSceneBuild()
	{
		// State
		m_SceneState = SceneState::kSceneIsInitializing;

		// entities
		for (UID const& entityUID : m_EntityUIDs)
			GetEntity(entityUID)->Init();
		// components
		for (UID const& componentUID : m_ComponentUIDs)
			GetComponent(componentUID)->Init();

		// Debug
		CHROMA_INFO_UNDERLINE;
		CHROMA_INFO("CHROMA SCENE:: Scene Successfully Loaded.");

		// timing
		m_SceneBuildEndTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(m_SceneBuildEndTime - m_SceneBuildStartTime);
		CHROMA_INFO("CHROMA SCENE:: Scene Load Took : {0} seconds", (float)duration.count()/1000.0f);
		CHROMA_INFO_UNDERLINE;

		// State
		m_SceneState = SceneState::kSceneBuilt;
	}

	void Scene::AddEntity(IEntity* const& newEntity)
	{
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsAdding)

		// collect UID
		m_EntityUIDs.insert(newEntity->GetUID());

		// collect entity
		m_Entities[newEntity->GetUID()] = newEntity;

		// State 
		SCENE_RESETSTATE
	}

	void Scene::AddAnimatedEntity(IEntity* const& newAnimatedEntity)
	{
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsAdding)

		// collect UID
		m_AnimatedEntityUIDs.insert(newAnimatedEntity->GetUID());

		// collect entity
		m_Entities[newAnimatedEntity->GetUID()] = newAnimatedEntity;

		// State 
		SCENE_RESETSTATE
	}

	void Scene::RemoveEntity(IEntity& RemoveEntity)
	{
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsRemoving)

		// Search for element 
		std::set<UID>::iterator it = m_EntityUIDs.find(RemoveEntity.GetUID());

		// Check if Iterator is valid
		if (it != m_EntityUIDs.end())
		{
			// Deletes the element pointing by iterator it
			m_EntityUIDs.erase(it);
			m_Entities.erase(RemoveEntity.GetUID());
		}
		// State 
		SCENE_RESETSTATE
	}

	float Scene::GetEntityDistanceToCamera(UID const& UID)
	{
		return glm::distance(GetEntity(UID)->GetTranslation(), m_RenderCamera->GetPosition());
	}


	float Scene::GetMeshComponentDistanceToCamera(const UID& uid)
	{
		return glm::distance(static_cast<MeshComponent*>(GetComponent(uid))->GetWSTranslation(), m_RenderCamera->GetPosition());
	}

	void Scene::AddAnimationComponent(IComponent* const& newAnimationComponent)
	{
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsAdding)

		// add to global component UIDs
		m_ComponentUIDs.insert(newAnimationComponent->GetUID());

		// collect component UID
		m_AnimationComponentUIDs.insert(newAnimationComponent->GetUID());

		// add component
		m_Components[newAnimationComponent->GetUID()] = newAnimationComponent;

		// State 
		SCENE_RESETSTATE
	}

	void Scene::AddCharacterControllerComponent(IComponent* const& newCharacterControllerComponent)
	{
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsAdding)

		// add to global component UIDs
		m_ComponentUIDs.insert(newCharacterControllerComponent->GetUID());

		// collect component UID
		m_CharacterControllerUIDs.insert(newCharacterControllerComponent->GetUID());

		// add component
		m_Components[newCharacterControllerComponent->GetUID()] = newCharacterControllerComponent;

		// State 
		SCENE_RESETSTATE
	}

	void Scene::AddMeshComponent(IComponent* const& newMeshComponent)
	{
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsAdding)

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

		// State 
		SCENE_RESETSTATE

	}

	void Scene::AddPhysicsComponent(IComponent* const& newPhysicsComponent)
	{
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsAdding)

		// add to global component UIDs
		m_ComponentUIDs.insert(newPhysicsComponent->GetUID());

		// collect component UID
		m_PhysicsComponentUIDs.insert(newPhysicsComponent->GetUID());

		// add component
		m_Components[newPhysicsComponent->GetUID()] = newPhysicsComponent;

		// State 
		SCENE_RESETSTATE
	}

	void Scene::AddUIComponent(IComponent* const& newUIComponent)
	{
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsAdding)

		// add to global component UIDs
		m_ComponentUIDs.insert(newUIComponent->GetUID());

		// collect component UID
		m_UIComponentUIDs.insert(newUIComponent->GetUID());

		// add component
		m_Components[newUIComponent->GetUID()] = newUIComponent;

		// State 
		SCENE_RESETSTATE
	}

	void Scene::AddStateMachineComponent(IComponent* const& newStateMachineComponent)
	{
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsAdding)

		// add to global component UIDs
		m_ComponentUIDs.insert(newStateMachineComponent->GetUID());

		// collect component UID
		m_StateMachineUIDs.insert(newStateMachineComponent->GetUID());

		// add component
		m_Components[newStateMachineComponent->GetUID()] = newStateMachineComponent;

		// State 
		SCENE_RESETSTATE
	}

	void Scene::AddAnimConstraintComponent(IComponent* const& newAnimConstraintComponent)
	{
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsAdding)

		// add to global component UIDs
		m_ComponentUIDs.insert(newAnimConstraintComponent->GetUID());

		// collect component UID
		m_AnimConstraintComponentUIDs.insert(newAnimConstraintComponent->GetUID());

		// add component
		m_Components[newAnimConstraintComponent->GetUID()] = newAnimConstraintComponent;

		// State 
		SCENE_RESETSTATE
	}

	void Scene::AddLight(IComponent* const& newLight)
	{
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsAdding)

		// add to global component UIDs
		m_ComponentUIDs.insert(newLight->GetUID());

		// collect component UID
		m_LightUIDs.insert(newLight->GetUID());

		// add component
		m_Components[newLight->GetUID()] = newLight;

		// State 
		SCENE_RESETSTATE
	}

	void Scene::RemoveLight(Light& RemoveLight)
	{
	
	}

	void Scene::SetLights(std::vector<Light*> newLights)
	{
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsAdding)

		for (Light*& light : newLights)
		{
			if (light->GetLightType() == Chroma::Type::Light::kSunlight)
			{
				m_SunLight = light;
				AddLight(light);
			}
			else
				AddLight(light);
		}

		// State 
		SCENE_RESETSTATE
	}

	void Scene::SetEntities(std::vector<IEntity*> const& newEntities)
	{
		// State 
		SCENE_TEMPSTATE(SceneState::kSceneIsAdding)

		for (IEntity* entity : newEntities)
			AddEntity(entity);

		// State 
		SCENE_RESETSTATE
	}
}

