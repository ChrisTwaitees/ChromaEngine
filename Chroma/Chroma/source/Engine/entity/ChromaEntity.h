#ifndef _CHROMA_ENTITY_
#define _CHROMA_ENTITY_

#include "../component/IChromaComponent.h"
#include "../memory/ChromaUID.h"
#include "texture/Texture.h"
#include "shader/Shader.h"
#include "light/Light.h"
#include "camera/Camera.h"
#include <vector>
#include <memory>

int findIndexInVector(const std::vector<IChromaComponent*>& vector, IChromaComponent*& element);

class ChromaEntity
{
	// Transforms
	glm::mat4 transformMatrix = glm::mat4(1);
	glm::mat4 identityMatrix = glm::mat4(1);
	// UID
	ChromaUID uid;
	// name
	std::string name;
public:
	// Component Vectors
	std::vector<IChromaComponent*> Components;
	std::vector<IChromaComponent*> RenderableComponents;
	std::vector<IChromaComponent*> LitComponents;
	std::vector<IChromaComponent*> ShadowCastingComponents;
	std::vector<IChromaComponent*> ForwardRenderComponents;
	std::vector<IChromaComponent*> TransparentComponents;
	std::vector<IChromaComponent*> UnLitComponents;


	// Add/Remove Components
	void addComponent(IChromaComponent*& newComponent);
	void removeComponent(IChromaComponent*& removeMe);

	// Transformations
	virtual void scale(glm::vec3 scalefactor);
	virtual void translate(glm::vec3 translatefactor);
	virtual void rotate(float degrees, glm::vec3 rotationaxis);
	
	virtual void setScale(glm::vec3 newscale);
	virtual void setPosition(glm::vec3 newposition);

	virtual glm::vec3 getPosition() { return glm::vec3(transformMatrix[3]); };

	// Getters and Setters
	std::string getUID() { return uid.UID; };
	std::string getName() { return name; };
	glm::mat4 getTransformationMatrix() { return transformMatrix; };
	
	void setName(std::string newName) { name = newName; };

	// Draw
	virtual void Draw(Shader& shader);
	virtual void Draw(Shader& shader, Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights);
	virtual void Draw(Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights);

	ChromaEntity();
	~ChromaEntity();
};

#endif