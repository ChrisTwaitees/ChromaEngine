#ifndef _CHROMA_SCENE_MANAGER_
#define _CHROMA_SCENE_MANAGER_

//stl
#include <vector>
#include <set>
#include <functional>
#include <chrono>
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
		// adding entities
		static void AddEntity(IEntity* const& newEntity);
		static void AddAnimatedEntity(IEntity* const& newAnimatedEntity);
		static void AddTransparentEntity(IEntity* const& newTransparentEntity);
		static void RemoveEntity(IEntity& RemoveEntity);
		static float GetEntityDistanceToCamera(UID const& UID);

		// adding components
		static void AddAnimationComponent(IComponent* const& newAnimationComponent);
		static void AddCharacterControllerComponent(IComponent* const& newCharacterControllerComponent);
		static void AddMeshComponent(IComponent* const& newMeshComponent);
		static void AddPhysicsComponent(IComponent* const& newPhysicsComponent);
		static void AddUIComponent(IComponent* const& newUIComponent);

		// lights
		static void AddLight(Light* const& newLight) { m_Lights.push_back(newLight); };
		static void RemoveLight(Light& RemoveLight);
		// setters
		static void SetEntities(std::vector<IEntity*> const& newEntities);

		static void SetSkyBox(SkyBox* const& newSkyBox) { m_Skybox = newSkyBox; };
		static void SetRenderCamera(Camera* const& newRenderCamera) { m_RenderCamera = newRenderCamera; };
		static void SetLights(std::vector<Light*> newLights);

		// getters
		// enitities
		static IEntity* GetEntity(UID const& UID);

		static inline std::set<UID>& GetEntityUIDs() { return m_EntityUIDs; }
		static inline std::set<UID>& GetTransparentEntityUIDs() { return m_TransparentEntityUIDs; }
		static inline std::set<UID>& GetAnimatedEntityUIDs() { return m_AnimatedEntityUIDs; }

		// components
		static std::map<UID, IComponent*>& GetAllComponents() { return m_Components; }
		static IComponent* GetComponent(UID const& UID);
		static void RemoveComponent(UID const& UID);
		static void SafeRemoveComponentUID(std::set<UID>& componentUIDList, UID const& removeUID);

		static inline std::set<UID>& GetMeshComponentUIDs() { return m_MeshComponentUIDs; }
		static inline std::set<UID>& GetSkinnedMeshComponentUIDs() { return m_SkinnedMeshComponentUIDs; }

		static inline std::set<UID>& GetAnimationComponentUIDs() { return m_AnimationComponentUIDs; }
		static inline std::set<UID>& GetCharacterControllerUIDs() { return m_CharacterControllerUIDs; }

		static inline std::set<UID>& GetRenderableComponentUIDs() { return m_RenderableComponentUIDs; }
		static inline std::set<UID>& GetLitComponentUIDs() { return m_LitComponentUIDs; }
		static inline std::set<UID>& GetShadowCastingComponentUIDs() { return m_ShadowCastingComponentUIDs; }
		static inline std::set<UID>& GetTransparentComponentUIDs() { return m_TransparentComponentUIDs; }
		static inline std::set<UID>& GetUnlitComponentUIDs() { return m_UnLitComponentUIDs; }
		static inline std::set<UID>& GetForwardLitComponentUIDs() { return m_ForwardLitComponentUIDs; }

		static inline std::set<UID>& GetPhysicsComponentUIDs() { return m_PhysicsComponentUIDs; }
		static inline std::set<UID>& GetUIComponentUIDs() { return m_UIComponentUIDs; }
		// Attrs
		static Camera*& GetRenderCamera() { return m_RenderCamera; };
		static std::vector<Light*>& GetLights() { return m_Lights; };
		static Light*& GetSunLight() { return m_SunLight; };
		static SkyBox*& GetSkyBox() { return m_Skybox; };
		static glm::vec3 GetAmbientColor() { return CalculateAmbientLightColor(); };
		static IBL*& GetIBL() { return m_IBL; };

	private:

		// scene components
		static std::vector<Light*> m_Lights;

		// Entities
		static std::map<UID, IEntity*> m_Entities;

		// Components
		static std::map<UID, IComponent*> m_Components;

		// UIDs
		// entities
		static std::set<UID> m_EntityUIDs;
		static std::set<UID> m_TransparentEntityUIDs;
		static std::set<UID> m_AnimatedEntityUIDs;
		// components
		// all components
		static std::set<UID> m_ComponentUIDs;
		// physics components
		static std::set<UID> m_PhysicsComponentUIDs;
		// mesh components
		static std::set<UID> m_MeshComponentUIDs;
		static void ProcessMeshComponentRenderFlags(IComponent* const& newMeshComponent);
		static std::set<UID> m_SkinnedMeshComponentUIDs;
		// animation components
		static std::set<UID> m_AnimationComponentUIDs;
		static std::set<UID> m_CharacterControllerUIDs;
		// render flagged components
		static std::set<UID> m_RenderableComponentUIDs;
		static std::set<UID> m_LitComponentUIDs;
		static std::set<UID> m_ForwardLitComponentUIDs;
		static std::set<UID> m_ShadowCastingComponentUIDs;
		static std::set<UID> m_TransparentComponentUIDs;
		static std::set<UID> m_UnLitComponentUIDs;
		// ui components
		static std::set<UID> m_UIComponentUIDs;

		// render components
		static Camera* m_RenderCamera;
		static Light* m_SunLight;
		static SkyBox* m_Skybox;
		static IBL* m_IBL; // image based lighting

		// functions
		static glm::vec3 CalculateAmbientLightColor();

		// timing
		static std::chrono::steady_clock::time_point m_SceneBuildStartTime;
		static std::chrono::steady_clock::time_point m_SceneBuildEndTime;
	};
}



#endif
