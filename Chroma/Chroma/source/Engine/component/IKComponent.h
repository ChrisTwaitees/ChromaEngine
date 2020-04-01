#ifndef CHROMA_IK_COMPONENT_H
#define CHROMA_IK_COMPONENT_H

#include <component/IComponent.h>

class IKComponent : public IComponent
{
public:

	// Component Functions
	virtual void Init();
	virtual void Update();
	virtual void Destroy();

	// Serialization
	virtual void Serialize(ISerializer*& serializer);
	virtual std::string GetTypeString() const;

	IKComponent() {};
	~IKComponent() {};
};

#endif // CHROMA_IK_COMPONENT_H