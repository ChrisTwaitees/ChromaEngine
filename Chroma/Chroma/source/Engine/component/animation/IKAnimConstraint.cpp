#include "IKAnimConstraint.h"
#include <entity/Entity.h>
#include <animation/SkeletonUtils.h>

void IKAnimConstraint::Init()
{
	CHROMA_TRACE("IK Anim Constraint : {0} initialized.", m_UID.data);
}

void IKAnimConstraint::Update()
{
	if (m_IsActive && m_ActivationAmount > 0.0)
	{
		CHROMA_INFO("IK Anim Constraint Updating");
		// fetch skeleton, iterate over ikconstraints
		for (std::pair<std::string, IKConstraint> ik : GetSkeleton()->GetIKConstraints())
		{
			/*glm::vec3 effectorPos(0.0, 5.0, 0.0);
			SetEffectorModelPos(ik.first, effectorPos);
			glm::mat4 effectorTransform(1.0);
			effectorTransform = glm::translate(effectorTransform, ik.second.m_EffectorWorldPos);
			GetSkeleton()->GetJointPtr(ik.second.m_EffectorJointID)->m_ModelSpaceTransform = effectorTransform;*/
			std::vector<Joint*> ikJoints = SkeletonUtils::GetInbetweenJoints(GetSkeleton(), ik.second.m_RootJointID, ik.second.m_EffectorJointID);
			CHROMA_INFO("");
		}
	}
	else
	{
		CHROMA_INFO("IK Anim Constraint Inactive : No updates");
	}
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

void IKAnimConstraint::SetEffectorWorldPos(std::string const& constraintName, glm::vec3 const& worldPos)
{
	GetConstraint(constraintName).m_EffectorWorldPos = (-GetParentEntity()->GetTranslation()) + worldPos;
}

void IKAnimConstraint::SetEffectorModelPos(std::string const& constraintName, glm::vec3 const& modelPos)
{
	GetConstraint(constraintName).m_EffectorWorldPos = modelPos;
}

IKConstraint& IKAnimConstraint::GetConstraint(std::string const& constraintName)
{
	if (GetSkeleton()->GetIKConstraints().find(constraintName) != GetSkeleton()->GetIKConstraints().end())
		return GetSkeleton()->GetIKConstraints().find(constraintName)->second;
	else
		CHROMA_ERROR("No Constraint of name : {} found!", constraintName);

}
