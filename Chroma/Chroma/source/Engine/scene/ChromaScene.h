#ifndef _CHROMA_SCENE_MANAGER_
#define _CHROMA_SCENE_MANAGER_

//stl
#include <vector>
//chroma
#include <camera/Camera.h>
#include <light/Light.h>
#include <physics/PhysicsEngine.h>
#include <model/SkyBox.h>
#include <ibl/IBL.h>
#include <time/ChromaTime.h>

// entity component
#include <entity/IChromaEntity.h>


class ChromaScene
{
	// scene components
	std::vector<Light*> m_Lights;

	std::vector<IChromaEntity*> m_Entities;
	std::vector<IChromaEntity*> m_TransparentEntities;

	PhysicsEngine* m_Physics;
	ChromaTime* m_Time;
	

	// render components
	Camera* m_RenderCamera{ new Camera() };
	Light* m_SunLight = new Light(Light::SUNLIGHT, glm::vec3(0.2, -0.8, 0.0), 1.0f);
	SkyBox* m_Skybox{ new SkyBox(m_RenderCamera) };
	IBL* m_IBL{ new IBL()}; // image based lighting

	// functions
	void Initialize();
	glm::vec3 CalculateAmbientLightColor();

public:
	// entities
	void AddEntity(IChromaEntity* const& newEntity);
	void RemoveEntity(IChromaEntity& RemoveEntity);
	// lights
	void AddLight(Light* const& newLight) { m_Lights.push_back(newLight); };
	void RemoveLight(Light& RemoveLight);
	// setters
	void SetSkyBox(SkyBox* const& newSkyBox) { m_Skybox = newSkyBox; };
	void SetRenderCamera(Camera* const& newRenderCamera) { m_RenderCamera = newRenderCamera; };
	void SetLights(std::vector<Light*> newLights);
	void SetEntities(std::vector<IChromaEntity*> const& newEntities);
	void SetPhysics(PhysicsEngine*& newPhysics) { m_Physics = newPhysics; };
	void SetTime(ChromaTime* const& newTime) { m_Time = newTime; };

	// getters
	std::vector<IChromaEntity*> GetEntities() { return m_Entities; };
	std::vector<IChromaEntity*> GetTransparentEntities() { return m_TransparentEntities; };
	Camera* GetRenderCamera() { return m_RenderCamera; };
	std::vector<Light*> GetLights() { return m_Lights; };
	Light* GetSunLight() { return m_SunLight; };
	PhysicsEngine* GetPhysics() { return  m_Physics; };
	ChromaTime* GetTime() { return m_Time; };
	SkyBox* GetSkyBox() { return m_Skybox; };
	glm::vec3 GetAmbientColor() { return CalculateAmbientLightColor(); };
	IBL* GetIBL() { return m_IBL; };

	ChromaScene();
	~ChromaScene();
};

#endif
