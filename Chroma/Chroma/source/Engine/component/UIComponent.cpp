#include "UIComponent.h"



void UIComponent::Init()
{
	CHROMA_TRACE("UI Component : {0} Initialized.", m_UID.data);
}


void UIComponent::Destroy()
{
	CHROMA_TRACE("UI Component : {0} Destroyed.", m_UID.data);
}

void UIComponent::Serialize(ISerializer*& serializer)
{
	CHROMA_INFO("Serializing UI Component : {0}", m_UID.data);
	serializer->StartObject("UIComponent", m_UID.data.c_str());
}

UIComponent::UIComponent()
{
}


UIComponent::~UIComponent()
{
}
