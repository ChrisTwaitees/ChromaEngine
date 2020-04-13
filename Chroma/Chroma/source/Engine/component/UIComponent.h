#ifndef _CHROMA_UICOMPONENT_H
#define _CHROMA_UICOMPONENT_H

#include <component/IComponent.h>

class UIComponent :	public IComponent
{
public:
	// Functions
	void Init();
	void Update() {};
	void Destroy();
	void Serialize(ISerializer*& serializer) override;

	virtual void Draw() {};

	UIComponent();
	virtual ~UIComponent();

protected:
	// Serialization
	Chroma::Type::Component m_Type{ Chroma::Type::Component::kUIComponent };
};

#endif