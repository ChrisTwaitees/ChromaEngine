#ifndef _CHROMA_COMPONENT_
#define _CHROMA_COMPONENT_

//stl
#include <string>
#include <typeinfo>

// chroma
#include <memory/ChromaUID.h>
#include <shader/Shader.h>
#include <camera/Camera.h>
#include <light/Light.h>
#include <texture/Texture.h>

// forward declarations
class IChromaEntity;

class IChromaComponent
{
protected:
	// name
	ChromaUID UID;
	std::string m_Name;

	// parent
	IChromaEntity* m_ParentEntity{ nullptr };

public:

	// Acessors
	ChromaUID GetUID() const { return UID; };

	std::string GetName() const { return m_Name; };
	void SetName(std::string newName) { m_Name = newName; };

	IChromaEntity* GetParentEntity() const {	return m_ParentEntity;	}
	virtual void SetParentEntity(IChromaEntity* const& parentEntity) { m_ParentEntity = parentEntity; }

	IChromaComponent() {};
	virtual ~IChromaComponent() {};
};

#endif