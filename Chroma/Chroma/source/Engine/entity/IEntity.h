#ifndef _CHROMA_ENTITY_INTERFACE_
#define _CHROMA_ENTITY_INTERFACE_

// stl
#include <string>
#include <vector>
// thirdparty
#include <glm/glm.hpp>
// chroma
#include <common/CoreCommon.h>
#include <model/Vertex.h>
#include <uid/UID.h>
#include <component/IComponent.h>

// forward declarations // 
class Scene;
class MeshComponent;
class PhysicsComponent;
class AnimationComponent;
class CharacterControllerComponent;

class IEntity
{
protected:
	// Transforms
	glm::mat4 m_Transform = glm::mat4(1);
	glm::mat4 m_identityMatrix = glm::mat4(1);
	glm::vec3 m_BBoxMin, m_BBoxMax;
	glm::vec3 m_Centroid;
	// UID
	std::string m_UID;
	// name
	std::string m_Name;
	// parent scene
	Scene* m_ParentScene;
	// Components
	virtual void addEmptyComponent(IComponent*& newComponent) = 0;
	virtual void addMeshComponent(MeshComponent*& newMeshComponent) = 0;
	virtual void addPhysicsComponent(PhysicsComponent*& newPhysicsComponent) = 0;
	virtual void addAnimationComponent(AnimationComponent*& newAnimationComponent) = 0;
	virtual void addCharacterControllerComponent(CharacterControllerComponent*& newCharacterControllerComponent) = 0;

	virtual void removeEmptyComponent(IComponent*& newComponent) = 0;
	virtual void removeMeshComponent(MeshComponent*& newMeshComponent) = 0;
	virtual void removePhysicsComponent(PhysicsComponent*& newPhysicsComponent) = 0;
	virtual void removeAnimationComponent(AnimationComponent*& oldAnimationComponent) = 0;
	virtual void removeCharacterControllerComponent(CharacterControllerComponent*& newCharacterControllerComponent) = 0;
public:
	// Name
	std::string GetUID() const { return m_UID; };
	std::string GetName() const { return m_Name; };
	void SetName(std::string newName) { m_Name = newName; };

	// Transformations
	virtual void Scale(glm::vec3 scalefactor) = 0;
	virtual void Translate(glm::vec3 translatefactor) = 0;
	virtual void Rotate(float degrees, glm::vec3 rotationaxis) = 0;

	// set
	virtual void SetTransform(glm::mat4 const& newTransformMat) = 0;
	virtual void SetScale(glm::vec3 const& newscale) = 0;
	virtual void SetPosition(glm::vec3 const& newposition) = 0;
	virtual void SetRotation(float const& degrees, glm::vec3 const& rotationaxis) = 0;
	
	// get
	virtual glm::mat4 GetTransform() = 0;
	virtual glm::vec3 GetPosition() = 0 ;
	virtual std::vector<ChromaVertex> GetVertices() = 0;
	virtual std::pair<glm::vec3, glm::vec3> GetBBox() = 0;
	virtual glm::vec3 GetCentroid() = 0;


	// Components
	virtual std::vector<IComponent*> getComponents() = 0;
	virtual std::vector<IComponent*> getRenderableComponents() = 0;
	virtual std::vector<IComponent*> getLitComponents() = 0;
	virtual std::vector<IComponent*> getShadowCastingComponents() = 0;
	virtual std::vector<IComponent*> GetTransparentComponents() = 0;
	virtual std::vector<IComponent*> getUnlitComponents() = 0;

	virtual std::vector<IComponent*> getMeshComponents() = 0;
	virtual std::vector<IComponent*> getPhysicsComponents() = 0;
	virtual std::vector<IComponent*> getAnimationComponents() = 0;
	virtual std::vector<IComponent*> getCharacterControllerComponents() = 0;

	// add
	template<typename component>
	void AddComponent(component newComponent) {
		CHROMA_FATAL("ComponentType not supported!" );
	};

	template<>
	void AddComponent<IComponent*>(IComponent* newComponent) {
		addEmptyComponent(newComponent);
	};

	template<>
	void AddComponent<MeshComponent*>(MeshComponent* newComponent) {
		addMeshComponent(newComponent);
	};

	template<>
	void AddComponent<PhysicsComponent*>(PhysicsComponent* newComponent) {
		addPhysicsComponent(newComponent);
	};

	template<>
	void AddComponent<AnimationComponent*>(AnimationComponent* newComponent) {
		addAnimationComponent(newComponent);
	};

	template<>
	void AddComponent<CharacterControllerComponent*>(CharacterControllerComponent* newComponent) {
		addCharacterControllerComponent(newComponent);
	};

	// remove
	template<typename component>
	void removeComponent(component newComponent) {
		CHROMA_FATAL("ComponentType not supported!");
	};

	template<>
	void removeComponent<IComponent*>(IComponent* newComponent) {
		removeEmptyComponent(newComponent);
	};

	template<>
	void removeComponent<MeshComponent*>(MeshComponent* newComponent) {
		removeMeshComponent(newComponent);
	};

	template<>
	void removeComponent<PhysicsComponent*>(PhysicsComponent* newComponent) {
		removePhysicsComponent(newComponent);
	};

	template<>
	void removeComponent<AnimationComponent*>(AnimationComponent* newComponent) {
		removeAnimationComponent(newComponent);
	};

	IEntity();
	virtual ~IEntity() {};
};

#endif