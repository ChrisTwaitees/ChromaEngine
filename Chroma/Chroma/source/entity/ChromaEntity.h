#ifndef _CHROMA_ENTITY_
#define _CHROMA_ENTITY_

#include "../component/ChromaComponent.h"
#include "../memory/ChromaUID.h"
#include "../texture/Texture.h"
#include "../shaders/Shader.h"
#include "../lights/Light.h"
#include "../cameras/Camera.h"
#include <vector>
#include <memory>

int findIndexInVector(const std::vector<ChromaComponent*>& vector, ChromaComponent*& element);

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
	std::vector<ChromaComponent*> Components;
	std::vector<ChromaComponent*> RenderableComponents;

	// Add/Remove Components
	void addComponent(ChromaComponent* newComponent);
	void removeComponent(ChromaComponent* removeMe);

	// Transformations
	virtual void scale(glm::vec3 scalefactor);
	virtual void translate(glm::vec3 translatefactor);
	virtual void rotate(float degrees, glm::vec3 rotationaxis);
	
	virtual void setScale(glm::vec3 newscale);
	virtual void setPosition(glm::vec3 newposition);

	// Getters and Setters
	std::string getUID() { return uid.UID; };
	std::string getName() { return name; };
	
	void setName(std::string newName) { name = newName; };

	// Draw
	virtual void Draw(Shader& shader);
	virtual void Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*>& Lights);
	virtual void Draw(Camera& RenderCamera, std::vector<Light*>& Lights);

	ChromaEntity();
	~ChromaEntity();
};

#endif