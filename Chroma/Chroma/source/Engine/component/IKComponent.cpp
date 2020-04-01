#include "IKComponent.h"

void IKComponent::Init()
{
	CHROMA_INFO("IK Component {0} Initialized!", m_UID.data);
}

void IKComponent::Update()
{
	CHROMA_INFO("Updating IK Component : {0}", m_UID.data);
}

void IKComponent::Destroy()
{
	CHROMA_INFO("IK Component {0} Destroyed!", m_UID.data);
}

void IKComponent::Serialize(ISerializer*& serializer)
{
	
}

std::string IKComponent::GetTypeString() const
{
	return "IK Component";
}
