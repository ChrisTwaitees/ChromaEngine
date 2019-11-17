#ifndef _CHROMA_ENTITY_INTERFACE_
#define _CHROMA_ENTITY_INTERFACE_

// stl
#include <string>
#include <vector>
// thirdparty
#include <glm/glm.hpp>
// chroma
#include <model/Vertex.h>
#include <memory/ChromaUID.h>
#include <component/IChromaComponent.h>

// forward declarations
class ChromaSceneManager;
class ChromaMeshComponent;
class ChromaPhysicsComponent;


class IChromaEntity
{
protected:
	// Transforms
	glm::mat4 m_transformMatrix = glm::mat4(1);
	glm::mat4 m_identityMatrix = glm::mat4(1);
	// UID
	ChromaUID m_uid;
	// name
	std::string m_name;
	// parent scene
	ChromaSceneManager* m_parentScene;

public:
	// Name
	virtual std::string getUID() = 0;
	virtual std::string getName() = 0;
	virtual void setName(std::string newName) = 0;

	// scene
	virtual ChromaSceneManager* getParentScene() = 0;
	virtual void bindParentScene(ChromaSceneManager* const& scene) = 0;

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

	virtual std::vector<IChromaComponent*> getMeshComponents() = 0;
	virtual std::vector<IChromaComponent*> getPhysicsComponents() = 0;

	virtual ~IChromaEntity() {};
};

#endif