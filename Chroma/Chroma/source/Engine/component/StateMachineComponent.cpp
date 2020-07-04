#include "StateMachineComponent.h"


namespace Chroma
{
	void StateMachineComponent::Init()
	{
		// Set Type
		m_Type = Chroma::Type::Component::kStateMachineComponent;

		CMPNT_INITIALIZED;
	}

	void StateMachineComponent::Update()
	{
		CMPNT_INITIALIZED
	}

	void StateMachineComponent::Destroy()
	{
		CMPNT_DESTROYED
	}

	void StateMachineComponent::Serialize(ISerializer*& serializer)
	{
		CMPNT_SERIALIZE_BEGIN

	}
}
