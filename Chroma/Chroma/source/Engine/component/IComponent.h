#ifndef _CHROMA_COMPONENT_
#define _CHROMA_COMPONENT_

//stl
#include <string>

// chroma
#include <uid/UID.h>

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
	virtual void Update() {};
	// Acessors
	UID GetUID() const { return m_UID; };

	std::string GetName() const { return m_Name; };
	void SetName(std::string newName) { m_Name = newName; };

	IEntity* GetParentEntity() const;
	virtual void SetParentEntityUID(UID const& newParentEntityUID) { m_ParentEntityUID = newParentEntityUID; }

	IComponent();
	virtual ~IComponent() {};
};

#endif