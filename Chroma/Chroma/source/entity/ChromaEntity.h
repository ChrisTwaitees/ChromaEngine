#ifndef _CHROMA_ENTITY_
#define _CHROMA_ENTITY_

#include "../texture/Texture.h"
#include "../shaders/Shader.h"
#include "../cameras/Camera.h"
#include "../lights/Light.h"
#include <vector>


class ChromaEntity
{
protected:
	// transforms
	glm::mat4 modelMat = glm::mat4(1);
	glm::mat4 identityMat = glm::mat4(1);

	// scene components
	Camera* pCamera;
	std::vector<Light>* pLights;


public:
	/*  Functions  */
	virtual void Draw(Shader& shader);
	virtual void Draw();

	// transform
	virtual void scale(glm::vec3 scalefactor);
	virtual void translate(glm::vec3 translatefactor);
	virtual void rotate(float degrees, glm::vec3 rotationaxis);
	
	virtual void setScale(glm::vec3 newscale);
	virtual void setPosition(glm::vec3 newposition);

	// assignments
	virtual void bindCamera(Camera* newCamera);
	virtual void bindLights(std::vector<Light>* newLights);
	virtual void bindTexture(Texture newTexture);
	virtual void bindTextures(std::vector<Texture> textures_val);

	ChromaEntity();
	~ChromaEntity();
};

#endif