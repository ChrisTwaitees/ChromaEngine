#include "UIComponent.h"



void UIComponent::Init()
{
	CHROMA_TRACE("UI Component : {0} Initialized.", m_UID.data);
}

void UIComponent::Update()
{
	CHROMA_TRACE("UI Component : {0} Updating.", m_UID.data);
}

void UIComponent::Destroy()
{
	CHROMA_TRACE("UI Component : {0} Destroyed.", m_UID.data);
}

UIComponent::UIComponent()
{
}


UIComponent::~UIComponent()
{
}
