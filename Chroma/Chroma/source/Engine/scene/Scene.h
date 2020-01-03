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



class Scene
{
	// scene components
	std::vector<Light*> m_Lights;

	// Entities
	std::map<std::string, IEntity*> m_Entities;
	std::map<std::string, IComponent*> m_UpdatingComponents;

	// UIDs
	std::set<std::string> m_EntityUIDs;
	std::set<std::string> m_TransparentEntityUIDs;
	std::set<std::string> m_AnimatedEntityUIDs;
	std::set<std::string> m_UpdatingComponentUIDs;

	// render components
	Camera* m_RenderCamera{ new Camera() };
	Light* m_SunLight = new Light(Light::SUNLIGHT, glm::vec3(0.2, -0.8, 0.0), 1.0f);
	SkyBox* m_Skybox{ new SkyBox(m_RenderCamera) };
	IBL* m_IBL{ new IBL()}; // image based lighting

	// functions
	void Initialize();
	void ProcessNewEntity(IEntity* const& newEntity);
	glm::vec3 CalculateAmbientLightColor();


public:
	// entities
	void AddEntity(IEntity* const& newEntity);
	void AddAnimatedEntity(IEntity* const& newAnimatedEntity) ;
	void AddTransparentEntity(IEntity* const& newTransparentEntity);
	void RemoveEntity(IEntity& RemoveEntity);

	// updating components
	void AddUpdatingComponent(IComponent* const& newUpdatingComponent);

	// lights
	void AddLight(Light* const& newLight) { m_Lights.push_back(newLight); };
	void RemoveLight(Light& RemoveLight);
	// setters
	void SetEntities(std::vector<IEntity*> const& newEntities);

	void SetSkyBox(SkyBox* const& newSkyBox) { m_Skybox = newSkyBox; };
	void SetRenderCamera(Camera* const& newRenderCamera) { m_RenderCamera = newRenderCamera; };
	void SetLights(std::vector<Light*> newLights);

	// getters
	// enitities
	IEntity* GetEntity(std::string UID);

	inline std::set<std::string> GetEntityUIDs() const { return m_EntityUIDs; }
	inline std::set<std::string> GetTransparentEntityUIDs() const { return m_TransparentEntityUIDs; }
	inline std::set<std::string> GetAnimatedEntityUIDs() const { return m_AnimatedEntityUIDs; }

	// components
	IComponent* GetComponent(std::string const& UID);

	inline std::set<std::string> GetUpdatingComponentUIDs() const { return m_UpdatingComponentUIDs; }


	Camera* GetRenderCamera() { return m_RenderCamera; };
	std::vector<Light*> GetLights() { return m_Lights; };
	Light* GetSunLight() { return m_SunLight; };
	SkyBox* GetSkyBox() { return m_Skybox; };
	glm::vec3 GetAmbientColor() { return CalculateAmbientLightColor(); };
	IBL* GetIBL() { return m_IBL; };

	Scene();
	~Scene();
};

#endif
