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
	std::string m_UID;
	std::string m_Name;

	// parent
	IEntity* m_ParentEntity{ nullptr };

public:
	virtual void Update() {};

	// Acessors
	std::string GetUID() const { return m_UID; };

	std::string GetName() const { return m_Name; };
	void SetName(std::string newName) { m_Name = newName; };

	IEntity* GetParentEntity() const {   return m_ParentEntity;	}
	virtual void SetParentEntity(IEntity* const& parentEntity) { m_ParentEntity = parentEntity; }

	IComponent();
	virtual ~IComponent() {};
};

#endif