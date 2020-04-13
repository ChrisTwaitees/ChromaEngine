#include "UIComponent.h"



void UIComponent::Init()
{
	CMPNT_INITIALIZED
}


void UIComponent::Destroy()
{
	CMPNT_DESTROYED
}

void UIComponent::Serialize(ISerializer*& serializer)
{
	CMPNT_SERIALIZE_BEGIN
}

UIComponent::UIComponent()
{
}


UIComponent::~UIComponent()
{
}
