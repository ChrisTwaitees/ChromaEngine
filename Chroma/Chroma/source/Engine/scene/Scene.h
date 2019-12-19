#ifndef _CHROMA_SCENE_MANAGER_
#define _CHROMA_SCENE_MANAGER_

//stl
#include <vector>
#include <set>
//chroma
#include <camera/Camera.h>
#include <light/Light.h>
#include <physics/PhysicsEngine.h>
#include <model/SkyBox.h>
#include <ibl/IBL.h>
#include <time/ChromaTime.h>

// entity component
#include <entity/IEntity.h>


class Scene
{
	// scene components
	std::vector<Light*> m_Lights;

	std::set<IEntity*> m_Entities;
	std::set<IEntity*> m_TransparentEntities;
	std::set<IEntity*> m_AnimatedEntities;

	PhysicsEngine* m_Physics;
	ChromaTime* m_Time;

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
	// lights
	void AddLight(Light* const& newLight) { m_Lights.push_back(newLight); };
	void RemoveLight(Light& RemoveLight);
	// setters
	void SetEntities(std::vector<IEntity*> const& newEntities);

	void SetSkyBox(SkyBox* const& newSkyBox) { m_Skybox = newSkyBox; };
	void SetRenderCamera(Camera* const& newRenderCamera) { m_RenderCamera = newRenderCamera; };
	void SetLights(std::vector<Light*> newLights);
	void SetPhysics(PhysicsEngine*& newPhysics) { m_Physics = newPhysics; };
	void SetTime(ChromaTime* const& newTime) { m_Time = newTime; };

	// getters
	std::set<IEntity*> GetEntities() { return m_Entities; };
	std::set<IEntity*> GetTransparentEntities() { return m_TransparentEntities; };
	std::set<IEntity*> GetAnimatedEntities() { return m_AnimatedEntities; };

	Camera* GetRenderCamera() { return m_RenderCamera; };
	std::vector<Light*> GetLights() { return m_Lights; };
	Light* GetSunLight() { return m_SunLight; };
	PhysicsEngine* GetPhysics() { return  m_Physics; };
	ChromaTime* GetTime() { return m_Time; };
	SkyBox* GetSkyBox() { return m_Skybox; };
	glm::vec3 GetAmbientColor() { return CalculateAmbientLightColor(); };
	IBL* GetIBL() { return m_IBL; };

	Scene();
	~Scene();
};

#endif
