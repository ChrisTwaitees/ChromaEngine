#include "IKAnimConstraint.h"

void IKAnimConstraint::Init()
{
	CHROMA_TRACE("IK Anim Constraint : {0} initialized.", m_UID.data);
}

void IKAnimConstraint::Update()
{
	CHROMA_INFO("IK Anim Constraint Updating");
}

void IKAnimConstraint::Destroy()
{
	CHROMA_TRACE("IK Anim Constraint : {0} Destroyed.", m_UID.data);
}

void IKAnimConstraint::Serialize(ISerializer*& serializer)
{
	CHROMA_INFO("IK Anim Constraint : {0} Serializing.", m_UID.data);
}

std::string IKAnimConstraint::GetTypeString() const
{
	return "IK Anim Constraint";
}
