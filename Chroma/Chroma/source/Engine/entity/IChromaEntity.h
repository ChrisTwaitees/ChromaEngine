#ifndef _CHROMA_ENTITY_INTERFACE_
#define _CHROMA_ENTITY_INTERFACE_

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <model/Vertex.h>
#include <memory/ChromaUID.h>
#include <component/IChromaComponent.h>
#include <component/ChromaMeshComponent.h>
#include <component/ChromaPhysicsComponent.h>

class IChromaEntity
{
protected:
	// Transforms
	glm::mat4 transformMatrix = glm::mat4(1);
	glm::mat4 identityMatrix = glm::mat4(1);
	// UID
	ChromaUID uid;
	// name
	std::string name;

	// Components
public:
	// Name
	virtual std::string getUID() = 0;
	virtual std::string getName() = 0;
	virtual void setName(std::string newName) = 0;

	// Transformations
	virtual void scale(glm::vec3 scalefactor) = 0;
	virtual void translate(glm::vec3 translatefactor) = 0;
	virtual void rotate(float degrees, glm::vec3 rotationaxis) = 0;

	// set
	virtual void setScale(glm::vec3 newscale) = 0;
	virtual void setPosition(glm::vec3 newposition) = 0;
	// get
	virtual glm::mat4 getTransformationMatrix() = 0;
	virtual glm::vec3 getPosition() = 0 ;

	// Component
	virtual std::vector<ChromaVertex> getVertices() = 0;

	virtual void addComponent(IChromaComponent* newComponent) = 0;
	virtual std::vector<IChromaComponent*> getComponents() = 0;
	virtual std::vector<IChromaComponent*> getRenderableComponents() = 0;
	virtual std::vector<IChromaComponent*> getLitComponents() = 0;
	virtual std::vector<IChromaComponent*> getShadowCastingComponents() = 0;
	virtual std::vector<IChromaComponent*> getTransparentComponents() = 0;
	virtual std::vector<IChromaComponent*> getUnlitComponents() = 0;

	virtual std::vector<ChromaMeshComponent*> getMeshComponents() = 0;
	virtual std::vector<ChromaPhysicsComponent*> getPhysicsComponents() = 0;

	virtual ~IChromaEntity() {};
};

#endif