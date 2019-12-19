#ifndef _CHROMA_COMPONENT_
#define _CHROMA_COMPONENT_

//stl
#include <string>
#include <typeinfo>

// chroma
#include <uid/UID.h>
#include <shader/Shader.h>
#include <camera/Camera.h>
#include <light/Light.h>
#include <texture/Texture.h>
#include <ChromaConfig.h>

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