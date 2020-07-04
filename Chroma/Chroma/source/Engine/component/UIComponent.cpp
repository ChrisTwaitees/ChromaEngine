#include "UIComponent.h"


namespace Chroma
{

	void UIComponent::Init()
	{
		// Set Type
		m_Type = Chroma::Type::Component::kUIComponent;

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
}
