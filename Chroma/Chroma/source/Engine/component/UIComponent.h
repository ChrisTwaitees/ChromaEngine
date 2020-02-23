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

	std::string GetTypeString() const override { return "UIComponent"; }

	virtual void Draw() {};

	UIComponent();
	virtual ~UIComponent();

};

#endif