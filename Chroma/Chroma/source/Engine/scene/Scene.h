#ifndef _CHROMA_SCENE_MANAGER_
#define _CHROMA_SCENE_MANAGER_

//common
#include <common/PrecompiledHeader.h>
//chroma
#include <camera/Camera.h>
#include <light/Light.h>
#include <model/SkyBox.h>
#include <ibl/IBL.h>
// entity component
#include <entity/IEntity.h>

// serialization
#include <serialization/FactorySerializer.h>

namespace Chroma
{
	class Scene
	{
	public:
		static void Init();
		static void PostSceneBuild();
		static void PreSceneBuild();

		// States
		enum SceneState {kSceneNotBuilt, kSceneIsBuilding, kSceneBuilt, kSceneIsAdding, kSceneIsRemoving, kSceneIsLoading, kSceneIsSaving, kSceneIsInitializing};
		static inline SceneState GetSceneState() { return m_SceneState; };

		// ENTITIES
		// funcs
		static float GetEntityDistanceToCamera(UID const& UID);

		// adding 
		static void AddEntity(IEntity* const& newEntity);
		static void AddAnimatedEntity(IEntity* const& newAnimatedEntity);
		static void RemoveEntity(IEntity& RemoveEntity);
		static void SetEntities(std::vector<IEntity*> const& newEntities);
		// get
		static inline std::set<UID>& GetEntityUIDs() { return m_EntityUIDs; }
		static inline std::set<UID>& GetAnimatedEntityUIDs() { return m_AnimatedEntityUIDs; }
		static std::map<UID, IEntity*>& GetAllEntities() { return m_Entities; }
		static IEntity* GetEntity(UID const& UID);
		static void RemoveEntity(UID const& UID);

		// COMPONENTS
		// funcs
		static float GetMeshComponentDistanceToCamera(const UID& uid);
		static void ProcessMeshComponentRenderFlags(IComponent* const& newMeshComponent);
		// adding 
		static void AddAnimationComponent(IComponent* const& newAnimationComponent);
		static void AddCharacterControllerComponent(IComponent* const& newCharacterControllerComponent);
		static void AddMeshComponent(IComponent* const& newMeshComponent);
		static void AddPhysicsComponent(IComponent* const& newPhysicsComponent);
		static void AddUIComponent(IComponent* const& newUIComponent);
		static void AddStateMachineComponent(IComponent* const& newStateMachineComponent);
		static void AddAnimConstraintComponent(IComponent* const& newIKComponent);
		// get
		static std::map<UID, IComponent*>& GetAllComponents() { return m_Components; }
		static IComponent* GetComponent(UID const& UID);
		static void RemoveComponent(UID const& UID);
		static void SafeRemoveComponentUID(std::set<UID>& componentUIDList, UID const& removeUID);

		static inline std::set<UID>& GetLightUIDs() { return m_LightUIDs; }

		static inline std::set<UID>& GetMeshComponentUIDs() { return m_MeshComponentUIDs; }
		static inline std::set<UID>& GetSkinnedMeshComponentUIDs() { return m_SkinnedMeshComponentUIDs; }

		static inline std::set<UID>& GetAnimationComponentUIDs() { return m_AnimationComponentUIDs; }
		static inline std::set<UID>& GetCharacterControllerUIDs() { return m_CharacterControllerUIDs; }
		static inline std::set<UID>& GetIKComponentUIDs() { return m_AnimConstraintComponentUIDs; }

		static inline std::set<UID>& GetRenderableComponentUIDs() { return m_RenderableComponentUIDs; }
		static inline std::set<UID>& GetLitComponentUIDs() { return m_LitComponentUIDs; }
		static inline std::set<UID>& GetShadowCastingComponentUIDs() { return m_ShadowCastingComponentUIDs; }
		static inline std::set<UID>& GetShadowReceivingComponentUIDs() { return m_ShadowReceivingComponentUIDs; }
		static inline std::set<UID>& GetTransparentComponentUIDs() { return m_TransparentComponentUIDs; }
		static inline std::set<UID>& GetUnlitComponentUIDs() { return m_UnLitComponentUIDs; }
		static inline std::set<UID>& GetForwardLitComponentUIDs() { return m_ForwardLitComponentUIDs; }

		static inline std::set<UID>& GetPhysicsComponentUIDs() { return m_PhysicsComponentUIDs; }
		static inline std::set<UID>& GetUIComponentUIDs() { return m_UIComponentUIDs; }

		// LIGHTS
		static void AddLight(IComponent* const& newLight);
		static void RemoveLight(Light& RemoveLight);
		static void SetLights(std::vector<Light*> newLights);
		static Light*& GetSunLight() { return m_SunLight; };
		static glm::vec3 GetAmbientColor() { return CalculateAmbientLightColor(); };
		static IBL*& GetIBL() { return m_IBL; };
		static void LoadIBL(std::string const& sourcePath);

		// SKYBOX
		static void SetSkyBox(SkyBox* const& newSkyBox) { m_Skybox = newSkyBox; };
		static SkyBox*& GetSkyBox() { return m_Skybox; };

		// SCENE GLOBALS
		static inline Texture& GetSceneNoiseTex() { return m_SceneNoise; };

		// CAMERA
		static void SetRenderCamera(Camera* const& newRenderCamera) { m_RenderCamera = newRenderCamera; };
		static Camera*& GetRenderCamera() { return m_RenderCamera; };

	private:
		// ENTITIES
		static std::map<UID, IEntity*> m_Entities;
		static std::set<UID> m_EntityUIDs;
		static std::set<UID> m_AnimatedEntityUIDs;

		// COMPONENTS
		static std::map<UID, IComponent*> m_Components;
 		static std::set<UID> m_ComponentUIDs;

		// physics components
		static std::set<UID> m_PhysicsComponentUIDs;
		// mesh components
		static std::set<UID> m_MeshComponentUIDs;
		static std::set<UID> m_SkinnedMeshComponentUIDs;
		// animation components
		static std::set<UID> m_AnimationComponentUIDs;
		static std::set<UID> m_CharacterControllerUIDs;
		static std::set<UID> m_AnimConstraintComponentUIDs;
		// render flagged components
		static std::set<UID> m_RenderableComponentUIDs;
		static std::set<UID> m_LitComponentUIDs;
		static std::set<UID> m_ForwardLitComponentUIDs;
		static std::set<UID> m_ShadowCastingComponentUIDs;
		static std::set<UID> m_ShadowReceivingComponentUIDs;
		static std::set<UID> m_TransparentComponentUIDs;
		static std::set<UID> m_UnLitComponentUIDs;
		// ui components
		static std::set<UID> m_UIComponentUIDs;
		// state machine components
		static std::set<UID> m_StateMachineUIDs;
		// light uids
		static std::set<UID> m_LightUIDs;

		// RENDER components
		static Camera* m_RenderCamera;
		static Light* m_SunLight;
		static SkyBox* m_Skybox;
		static IBL* m_IBL; // image based lighting

		// Scene Globals
		static Texture m_SceneNoise;

		// funcs
		static glm::vec3 CalculateAmbientLightColor();

		// timing
		static std::chrono::steady_clock::time_point m_SceneBuildStartTime;
		static std::chrono::steady_clock::time_point m_SceneBuildEndTime;

		// Scene State
		static SceneState m_SceneState;
	};

#define SCENE_TEMPSTATE(...)		SceneState prevState = m_SceneState; m_SceneState = (__VA_ARGS__);
#define SCENE_RESETSTATE			m_SceneState = prevState;
}

#endif
