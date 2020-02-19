#ifndef _CHROMA_UICOMPONENT_H
#define _CHROMA_UICOMPONENT_H

#include <component/IComponent.h>

class UIComponent :	public IComponent
{
public:
	// Functions
	virtual void Init();
	virtual void Update() {};
	virtual void Destroy();

	virtual void Draw() {};

	UIComponent();
	virtual ~UIComponent();

};

#endif