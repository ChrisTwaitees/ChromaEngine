#ifndef _CHROMA_SCENE_MANAGER_
#define _CHROMA_SCENE_MANAGER_

//stl
#include <vector>
//chroma
#include <camera/Camera.h>
#include <light/Light.h>
// entity component
#include <entity/IChromaEntity.h>
class ChromaMeshComponent;

class ChromaScene
{
private:
	// scene components
	std::vector < std::shared_ptr<Light>> m_lights;
	std::vector<IChromaEntity*> m_entities;
	std::vector<IChromaEntity*> m_transparentEntities;

	Camera* m_renderCamera{ new Camera };
	Light* m_sunLight = new Light(Light::SUNLIGHT, glm::vec3(0.2, -0.8, 0.0), 1.0f);

public:
	void addEntity(IChromaEntity*& newEntity);
	void removeEntity(IChromaEntity& removeEntity);

	void addLight(std::shared_ptr<Light>& newLight) { m_lights.push_back(newLight); };
	void removeLight(Light& removeLight);

	// setters
	void setRenderCamera(Camera*& newRenderCamera) { m_renderCamera = newRenderCamera; };
	void setLights(std::vector<std::shared_ptr<Light>>& newLights);
	void setEntities(std::vector<IChromaEntity*>& newCameras) { m_entities = newCameras; };

	// getters
	std::vector<IChromaEntity*> getEntities() { return m_entities; };
	std::vector<IChromaEntity*> getTransparentEntities() { return m_transparentEntities; };
	Camera* getRenderCamera() { return m_renderCamera; };
	std::vector<std::shared_ptr<Light>>& getLights() { return m_lights; };
	Light* getSunLight() { return m_sunLight; };


	ChromaScene();
	~ChromaScene();
};

#endif
