#ifndef _CHROMA_COMPONENT_
#define _CHROMA_COMPONENT_

//common
#include <common/PrecompiledHeader.h>

// chroma
#include <uid/UID.h>
#include <common/CoreCommon.h>
#include <serialization/ISerializer.h>

// forward declarations
class IEntity;

class IComponent
{
protected:
	// name
	UID m_UID;
	std::string m_Name;

	// parent
	UID m_ParentEntityUID;

public:
	// Functions
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;
	
	// Serialization
	virtual void Serialize(ISerializer*& serializer) = 0;
	virtual std::string GetTypeString() const = 0;

	// Acessors
	UID GetUID() const { return m_UID; };

	std::string GetName() const { return m_Name; };
	void SetName(std::string newName) { m_Name = newName; };


	// Entity
	IEntity* GetParentEntity() const;
	UID GetParentEntityUID() const;
	std::string GetParentEntityName() const;
	virtual void SetParentEntityUID(UID const& newParentEntityUID) { m_ParentEntityUID = newParentEntityUID; }

	IComponent();
	virtual ~IComponent() {};
};

#endif