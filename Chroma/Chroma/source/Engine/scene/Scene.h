#ifndef _CHROMA_SCENE_MANAGER_
#define _CHROMA_SCENE_MANAGER_

//stl
#include <vector>
#include <set>
#include <functional>
//chroma
#include <camera/Camera.h>
#include <light/Light.h>
#include <model/SkyBox.h>
#include <ibl/IBL.h>
// entity component
#include <entity/IEntity.h>

namespace Chroma
{
	class Scene
	{
		// scene components
		static std::vector<Light*> m_Lights;

		// Entities
		static std::map<std::string, IEntity*> m_Entities;
		static std::map<std::string, IComponent*> m_UpdatingComponents;

		// UIDs
		static std::set<std::string> m_EntityUIDs;
		static std::set<std::string> m_TransparentEntityUIDs;
		static std::set<std::string> m_AnimatedEntityUIDs;
		static std::set<std::string> m_UpdatingComponentUIDs;

		// render components
		static Camera* m_RenderCamera;
		static Light* m_SunLight;
		static SkyBox* m_Skybox;
		static IBL* m_IBL; // image based lighting

		// functions
		static glm::vec3 CalculateAmbientLightColor();

	public:
		static void Init();
		static void PostSceneBuild();
		// entities
		static void AddEntity(IEntity* const& newEntity);
		static void AddAnimatedEntity(IEntity* const& newAnimatedEntity);
		static void AddTransparentEntity(IEntity* const& newTransparentEntity);
		static void RemoveEntity(IEntity& RemoveEntity);
		static float GetEntityDistanceToCamera(std::string const& UID);

		// updating components
		static void AddUpdatingComponent(IComponent* const& newUpdatingComponent);

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
		static IEntity* GetEntity(std::string UID);

		static inline std::set<std::string>& GetEntityUIDs() { return m_EntityUIDs; }
		static inline std::set<std::string>& GetTransparentEntityUIDs() { return m_TransparentEntityUIDs; }
		static inline std::set<std::string>& GetAnimatedEntityUIDs() { return m_AnimatedEntityUIDs; }

		// components
		static IComponent* GetComponent(std::string const& UID);

		static inline std::set<std::string>& GetUpdatingComponentUIDs() {return m_UpdatingComponentUIDs; }

		// Attrs
		static Camera*& GetRenderCamera() { return m_RenderCamera; };
		static std::vector<Light*>& GetLights() { return m_Lights; };
		static Light*& GetSunLight() { return m_SunLight; };
		static SkyBox*& GetSkyBox() { return m_Skybox; };
		static glm::vec3 GetAmbientColor() { return CalculateAmbientLightColor(); };
		static IBL*& GetIBL() { return m_IBL; };
	};
}



#endif
