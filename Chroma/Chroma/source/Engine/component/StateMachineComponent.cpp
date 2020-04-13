#include "StateMachineComponent.h"

void StateMachineComponent::Init()
{
	CHROMA_INFO("STATE MACHINE :: Initialized");
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
