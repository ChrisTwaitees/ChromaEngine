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
class ChromaScene;
class ChromaMeshComponent;
class ChromaPhysicsComponent;


class IChromaEntity
{
protected:
	// Transforms
	glm::mat4 m_transformMatrix = glm::mat4(1);
	glm::mat4 m_identityMatrix = glm::mat4(1);
	glm::vec3 m_BBoxMin, m_BBoxMax;
	glm::vec3 m_Centroid;
	// UID
	ChromaUID m_uid;
	// name
	std::string m_Name;
	// parent scene
	ChromaScene* m_parentScene;
	// Components
	virtual void addEmptyComponent(IChromaComponent*& newComponent) = 0;
	virtual void addMeshComponent(ChromaMeshComponent*& newMeshComponent) = 0;
	virtual void addPhysicsComponent(ChromaPhysicsComponent*& newPhysicsComponent) = 0;
	virtual void removeEmptyComponent(IChromaComponent*& newComponent) = 0;
	virtual void removeMeshComponent(ChromaMeshComponent*& newMeshComponent) = 0;
	virtual void removePhysicsComponent(ChromaPhysicsComponent*& newPhysicsComponent) = 0;

public:
	// Name
	virtual ChromaUID GetUID() = 0;
	virtual std::string GetName() = 0;
	virtual void SetName(std::string newName) = 0;

	// scene
	virtual ChromaScene* getParentScene() = 0;
	virtual void bindParentScene(ChromaScene* const& scene) = 0;

	// Transformations
	virtual void scale(glm::vec3 scalefactor) = 0;
	virtual void translate(glm::vec3 translatefactor) = 0;
	virtual void rotate(float degrees, glm::vec3 rotationaxis) = 0;

	// set
	virtual void setTransformMatrix(glm::mat4 newTransformMat) = 0;
	virtual void setScale(glm::vec3 newscale) = 0;
	virtual void SetPosition(glm::vec3 newposition) = 0;
	virtual void setRotation(float degrees, glm::vec3 rotationaxis) = 0;
	
	// get
	virtual glm::mat4 GetTransformationMatrix() = 0;
	virtual glm::vec3 GetPosition() = 0 ;
	virtual std::vector<ChromaVertex> GetVertices() = 0;
	virtual std::pair<glm::vec3, glm::vec3> GetBBox() = 0;
	virtual glm::vec3 GetCentroid() = 0;


	// Components
	virtual std::vector<IChromaComponent*> getComponents() = 0;
	virtual std::vector<IChromaComponent*> getRenderableComponents() = 0;
	virtual std::vector<IChromaComponent*> getLitComponents() = 0;
	virtual std::vector<IChromaComponent*> getShadowCastingComponents() = 0;
	virtual std::vector<IChromaComponent*> getTransparentComponents() = 0;
	virtual std::vector<IChromaComponent*> getUnlitComponents() = 0;

	virtual std::vector<IChromaComponent*> getMeshComponents() = 0;
	virtual std::vector<IChromaComponent*> getPhysicsComponents() = 0;


	// add
	template<typename component>
	void addComponent(component newComponent) {
		std::cout << "ComponentType not supported" << std::endl;
	};

	template<>
	void addComponent<IChromaComponent*>(IChromaComponent* newComponent) {
		addEmptyComponent(newComponent);
	};

	template<>
	void addComponent<ChromaMeshComponent*>(ChromaMeshComponent* newComponent) {
		addMeshComponent(newComponent);
	};

	template<>
	void addComponent<ChromaPhysicsComponent*>(ChromaPhysicsComponent* newComponent) {
		addPhysicsComponent(newComponent);
	};

	// remove
	template<typename component>
	void removeComponent(component newComponent) {
		std::cout << "ComponentType not supported" << std::endl;
	};

	template<>
	void removeComponent<IChromaComponent*>(IChromaComponent* newComponent) {
		removeEmptyComponent(newComponent);
	};

	template<>
	void removeComponent<ChromaMeshComponent*>(ChromaMeshComponent* newComponent) {
		removeMeshComponent(newComponent);
	};

	template<>
	void removeComponent<ChromaPhysicsComponent*>(ChromaPhysicsComponent* newComponent) {
		removePhysicsComponent(newComponent);
	};


	virtual ~IChromaEntity() {};
};

#endif