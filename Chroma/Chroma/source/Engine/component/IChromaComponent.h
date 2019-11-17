#ifndef _CHROMA_COMPONENT_
#define _CHROMA_COMPONENT_
#include <string>
#include <typeinfo>

// chroma

#include <memory/ChromaUID.h>
#include <shader/Shader.h>
#include <camera/Camera.h>
#include <light/Light.h>
#include <texture/Texture.h>
#include <entity/IChromaEntity.h>



class IChromaComponent
{
protected:
	// name
	ChromaUID uid;
	std::string name;
	std::string typeName;

	// parent
	IChromaEntity* m_parentEntity;

public:
	// name / uid / type
	std::string getUID() { return uid.UID; };
	std::string getName() { return name; };
	void setName(std::string newName) { name = newName; };
	std::string getType() { return typeName; }

	// parent
	IChromaEntity* getParentEntity() {	return m_parentEntity;	}
	virtual void bindParentEntity(IChromaEntity* const& parentEntity) { m_parentEntity = parentEntity; }

	IChromaComponent();
	virtual ~IChromaComponent();
};

#endif