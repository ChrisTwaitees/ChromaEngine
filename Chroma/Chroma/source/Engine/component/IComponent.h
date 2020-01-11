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
	std::string m_ParentEntityUID;

public:
	virtual void Update() {};
	// Acessors
	std::string GetUID() const { return m_UID; };

	std::string GetName() const { return m_Name; };
	void SetName(std::string newName) { m_Name = newName; };

	IEntity* GetParentEntity() const;
	virtual void SetParentEntityUID(std::string const& newParentEntityUID) { m_ParentEntityUID = newParentEntityUID; }

	virtual glm::mat4 GetTransform() { return glm::mat4(1); };

	IComponent();
	virtual ~IComponent() {};
};

#endif