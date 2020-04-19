#ifndef _CHROMA_ENTITY_INTERFACE_
#define _CHROMA_ENTITY_INTERFACE_

//common
#include <common/PrecompiledHeader.h>
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
class StateMachineComponent;
class AnimConstraintComponent;

class IEntity
{
public:
	// Init
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;
	virtual void Serialize(ISerializer*& serializer) = 0;
	
	// Type
	Chroma::Type::Entity GetType() { return m_Type; };
	std::string GetTypeName() { return Chroma::Type::GetName(m_Type); }

	//UID
	UID GetUID() const { return m_UID; };
	void SetUID(const UID& newUID) { m_UID = newUID; };

	// Name
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
	virtual glm::vec3& GetCentroid() = 0;
	virtual float GetHeight() = 0;


	// Components
	inline virtual std::vector<UID> GetComponentUIDs() { return m_ComponentUIDs; }

	inline virtual std::vector<UID> GetMeshComponentUIDs() { return m_MeshComponentUIDs; }
	inline virtual void AddMeshComponentUID(const UID& newUID) { m_MeshComponentUIDs.push_back(newUID); }

	inline virtual std::vector<UID> GetPhysicsComponentUIDs() { return m_PhysicsComponentUIDs; }
	inline virtual void AddPhysicsComponentUID(const UID& newUID) { m_PhysicsComponentUIDs.push_back(newUID); }

	inline virtual std::vector<UID> GetAnimationComponentUIDs() { return m_AnimationComponentUIDs; }
	inline virtual void AddAnimationComponentUID(const UID& newUID) { m_AnimationComponentUIDs.push_back(newUID); }

	inline virtual std::vector<UID> GetCharacterControllerComponentUIDs() { return m_CharacterControllerComponentUIDs; }
	inline virtual void AddCharacterControllerComponentUID(const UID& newUID) { m_CharacterControllerComponentUIDs.push_back(newUID); }

	inline virtual std::vector<UID> GetAnimConstraintComponentUIDs() { return m_AnimConstraintComponentUIDs; }
	inline virtual void AddAnimConstraintComponentUID(const UID& newUID) { m_AnimConstraintComponentUIDs.push_back(newUID); }

	inline virtual std::vector<UID> GetStateMachineComponentUIDs() { return m_StateMachineComponentUIDs; }
	inline virtual void AddStateMachineComponentUID(const UID& newUID) { m_StateMachineComponentUIDs.push_back(newUID); }

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

	template<>
	void AddComponent<StateMachineComponent*>(StateMachineComponent* newComponent) {
		AddStateMachineComponent(newComponent);
	}

	template<>
	void AddComponent<AnimConstraintComponent*>(AnimConstraintComponent* newComponent) {
		AddAnimConstraintComponent(newComponent);
	}


	IEntity();
	virtual ~IEntity() {};

protected:
	// UID
	UID m_UID;

	// Name
	std::string m_Name;
	// Serialization
	Chroma::Type::Entity m_Type{ Chroma::Type::Entity::kIEntity };

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
	std::vector<UID> m_StateMachineComponentUIDs;
	std::vector<UID> m_AnimConstraintComponentUIDs;

	// add
	virtual void AddComponent(IComponent*& newComponent) = 0;
	virtual void AddMeshComponent(MeshComponent*& newMeshComponent) = 0;
	virtual void AddPhysicsComponent(PhysicsComponent*& newPhysicsComponent) = 0;
	virtual void AddAnimationComponent(AnimationComponent*& newAnimationComponent) = 0;
	virtual void AddCharacterControllerComponent(CharacterControllerComponent*& newCharacterControllerComponent) = 0;
	virtual void AddStateMachineComponent(StateMachineComponent*& newStateMachineComponent) = 0;
	virtual void AddAnimConstraintComponent(AnimConstraintComponent*& newIKComponent) = 0;
};

#ifdef DEBUG
#define ENTITY_SERIALIZE_BEGIN 	CHROMA_INFO("{} Serializing, UID : {}", GetTypeName(), m_UID.data); serializer->StartObject(GetType(), m_UID);
#define ENTITY_DESTROYED CHROMA_INFO("{} Destroyed. UID : {}", GetTypeName(), m_UID.data );
#define ENTITY_INITIALIZED CHROMA_INFO("{} Intialized. UID : {}", GetTypeName(), m_UID.data );
#else
#define ENTITY_SERIALIZE_BEGIN 	serializer->StartObject(GetType(), m_UID);
#define ENTITY_DESTROYED
#define ENTITY_INITIALIZED
#endif

#endif