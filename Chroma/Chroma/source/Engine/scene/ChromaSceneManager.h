#ifndef _CHROMA_SCENE_
#define _CHROMA_SCENE_

#include <camera/Camera.h>
#include <light/Light.h>
#include <entity/IChromaEntity.h>
#include <component/IChromaComponent.h>
#include <vector>


class ChromaSceneManager
{
public:
	// scene components
	std::vector < std::shared_ptr<Light>> Lights;
	std::vector<IChromaEntity*> Entities;
	std::vector<IChromaEntity*> TransparentEntities;

	Camera* RenderCamera{ new Camera };
	Light* SunLight = new Light(Light::SUNLIGHT, glm::vec3(0.2, -0.8, 0.0), 1.0f);

	void addEntity(IChromaEntity*& newEntity) { Entities.push_back(newEntity); };
	void removeEntity(IChromaEntity& removeEntity);

	void addLight(std::shared_ptr<Light>& newLight) { Lights.push_back(newLight); };
	void removeLight(Light& removeLight);

	// setters
	void setRenderCamera(Camera*& newRenderCamera) { RenderCamera = newRenderCamera; };
	void setLights(std::vector<std::shared_ptr<Light>>& newLights);
	void setEntities(std::vector<IChromaEntity*>& newCameras) { Entities = newCameras; };

	// getters
	Camera* getRenderCamera() { return RenderCamera; };
	std::vector<std::shared_ptr<Light>>& getLights() { return Lights; };

	// functions
	IChromaEntity* addNewEntity(IChromaComponent* component);

	ChromaSceneManager();
	~ChromaSceneManager();
};

#endif
