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
	// UID
	UID m_UID;

	// Name
	std::string m_Name;

	// Transforms
	glm::mat4 m_Transform{ glm::mat4(1.0f) };
	glm::vec3 m_Translation{ glm::vec3(0.0f) };
	glm::quat m_Rotation{ glm::quat() };
	glm::vec3 m_Scale{ glm::vec3(1.0f) };
	virtual void RebuildTransform();

	// BBOX
	glm::vec3 m_BBoxMin, m_BBoxMax;
	glm::vec3 m_Centroid;
	virtual void CalculateBBox() {};
	virtual void CalculateCentroid() {};

	// Components
	// store
	std::vector<UID> m_ComponentUIDs;
	std::vector<UID> m_MeshComponentUIDs;
	std::vector<UID> m_PhysicsComponentUIDs;
	std::vector<UID> m_AnimationComponentUIDs;
	std::vector<UID> m_CharacterControllerComponentUIDs;
	// add
	virtual void AddComponent(IComponent*& newComponent) = 0;
	virtual void AddMeshComponent(MeshComponent*& newMeshComponent) = 0;
	virtual void AddPhysicsComponent(PhysicsComponent*& newPhysicsComponent) = 0;
	virtual void AddAnimationComponent(AnimationComponent*& newAnimationComponent) = 0;
	virtual void AddCharacterControllerComponent(CharacterControllerComponent*& newCharacterControllerComponent) = 0;

public:
	// Init
	virtual void Init();

	// Name
	UID GetUID() const { return m_UID; };
	std::string GetName() const { return m_Name; };
	void SetName(std::string newName) { m_Name = newName; };

	// Transformations
	// additive
	virtual void Scale(glm::vec3 scalefactor) = 0;
	virtual void Translate(glm::vec3 translatefactor) = 0;
	virtual void Rotate(float degrees, glm::vec3 rotationaxis) = 0;

	// set
	virtual void SetTransform(glm::mat4 const& newTransformMat);
	virtual inline void SetScale(glm::vec3 const& newscale) { m_Scale = newscale; RebuildTransform();}
	virtual inline void SetTranslation(glm::vec3 const& newposition) { m_Translation = newposition; RebuildTransform(); }
	virtual void SetRotation(glm::quat const& newRotation) { m_Rotation = newRotation; RebuildTransform(); }
	
	// get
	virtual inline glm::mat4 GetTransform() const { return m_Transform; };
	virtual glm::vec3 GetTranslation() const { return m_Translation; };
	virtual glm::vec3 GetScale() const { return m_Scale; };
	virtual glm::quat GetRotation() const { return m_Rotation; };

	// BBOX
	virtual std::vector<ChromaVertex> GetVertices() = 0;
	virtual std::pair<glm::vec3, glm::vec3> GetBBox() = 0;
	virtual glm::vec3 GetCentroid() = 0;


	// Components
	inline virtual std::vector<UID> getComponentUIDs() { return m_ComponentUIDs; }
	inline virtual std::vector<UID> getMeshComponentUIDs() { return m_MeshComponentUIDs; }
	inline virtual std::vector<UID> getPhysicsComponentUIDs() { return m_PhysicsComponentUIDs; }
	inline virtual std::vector<UID> getAnimationComponentUIDs() { return m_AnimationComponentUIDs; }
	inline virtual std::vector<UID> getCharacterControllerComponentUIDs() { return m_CharacterControllerComponentUIDs; }

	// add
	template<typename component>
	void AddComponent(component newComponent) {
		CHROMA_FATAL("ComponentType not supported!" );
	};

	template<>
	void AddComponent<IComponent*>(IComponent* newComponent) {
		AddComponent(newComponent);
	};

	template<>
	void AddComponent<MeshComponent*>(MeshComponent* newComponent) {
		AddMeshComponent(newComponent);
	};

	template<>
	void AddComponent<PhysicsComponent*>(PhysicsComponent* newComponent) {
		AddPhysicsComponent(newComponent);
	};

	template<>
	void AddComponent<AnimationComponent*>(AnimationComponent* newComponent) {
		AddAnimationComponent(newComponent);
	};

	template<>
	void AddComponent<CharacterControllerComponent*>(CharacterControllerComponent* newComponent) {
		AddCharacterControllerComponent(newComponent);
	};


	IEntity();
	virtual ~IEntity() {};
};

#endif