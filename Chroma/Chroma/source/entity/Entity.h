#ifndef _CHROMA_ENTITY_
#define _CHROMA_ENTITY_

#include "../textures/Texture.h"
#include "../shaders/Shader.h"
#include "../cameras/Camera.h"
#include "../lights/Light.h"
#include <vector>

class Entity
{
protected:
	// transforms
	glm::mat4 modelMat = glm::mat4(1);

	// scene components
	Camera* pCamera;
	std::vector<Light>* pLights;

public:
	/*  Functions  */
	virtual void Draw(Shader& shader);
	virtual void Draw();


	// assignments
	virtual void bindCamera(Camera* newCamera);
	virtual void bindLights(std::vector<Light>* newLights);
	virtual void bindTexture(Texture newTexture);
	virtual void bindTextures(std::vector<Texture> textures_val);

	Entity();
	~Entity();
};

#endif