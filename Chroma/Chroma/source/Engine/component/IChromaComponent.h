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
	ChromaUID uid;
	std::string name;

	// parent
	IChromaEntity* m_parentEntity;
public:
	// name / uid / type
	ChromaUID getUID() { return uid; };
	std::string getName() { return name; };
	void SetName(std::string newName) { name = newName; };

	// parent
	IChromaEntity* getParentEntity() {	return m_parentEntity;	}
	virtual void bindParentEntity(IChromaEntity* const& parentEntity) { m_parentEntity = parentEntity; }

	IChromaComponent() {};
	virtual ~IChromaComponent() {};
};

#endif