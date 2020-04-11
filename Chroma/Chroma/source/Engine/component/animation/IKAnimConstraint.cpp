#include "IKAnimConstraint.h"
#include <entity/Entity.h>
#include <animation/SkeletonUtils.h>
#include <animation/Animator.h>

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
		for (auto& ik : GetSkeleton()->GetIKConstraints())
		{
			SolveIK(ik.second);
		}
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

void IKAnimConstraint::SolveIK(IKConstraint const& ik)
{
	// TODO : Implement Rotation and PoleVector : https://www.youtube.com/watch?v=qqOAzn05fvk&t=393s
	std::vector<glm::vec3> jntPositionsWS;
	glm::vec3 entityScale = GetParentEntity()->GetScale();

	// get positions
	for (int i = 0; i < ik.m_JointIDs.size(); i++)
	{
		jntPositionsWS.push_back( Chroma::Math::GetTranslation(GetSkeleton()->GetRootTransform() * GetSkeleton()->GetJointPtr(ik.m_JointIDs[i])->m_ModelSpaceTransform));
	}

	// root to effector
	float distToEffector = glm::distance(jntPositionsWS[0], ik.m_EffectorWorldPos);

	// calculations
	// check if effector within solve range
	if (distToEffector >= ik.m_ChainLength * entityScale.x)
	{
		glm::vec3 toEffector = glm::normalize(ik.m_EffectorWorldPos - jntPositionsWS[0]);
		for (int i = 1; i < ik.m_JointIDs.size(); i++)
		{
			jntPositionsWS[i] = jntPositionsWS[i-1] + (toEffector * ik.m_JointDistances[i] * entityScale);
		}
	}
	else // solve using Fabrik Algorithm : https://github.com/ditzel/SimpleIK
	{
		for (unsigned int i = 0; i < m_Iterations; i++)
		{
			// backwards
			for (int i = ik.m_JointIDs.size() - 1; i > 0; i--)
			{
				if (i == ik.m_JointIDs.size() - 1)
					jntPositionsWS[i] = ik.m_EffectorWorldPos; // set to effector
				else
					jntPositionsWS[i] = jntPositionsWS[i + 1] + ((glm::normalize(jntPositionsWS[i] - jntPositionsWS[i + 1]) * ik.m_JointDistances[i] * entityScale));
			}

			// forwards
			for (int i = 1; i < jntPositionsWS.size(); i++)
				jntPositionsWS[i] = jntPositionsWS[i - 1] + ((glm::normalize(jntPositionsWS[i] - jntPositionsWS[i - 1]) * ik.m_JointDistances[i] * entityScale));

			// within delta threshold?
			if (distToEffector < m_DeltaThreshold)
				break;
		}
	}

	// set positions
	for (int i = 0; i < ik.m_JointIDs.size(); i++)
	{
		glm::mat4 newMSJointTrs = glm::translate(glm::inverse(GetSkeleton()->GetRootTransform()) * glm::mat4(1.0), jntPositionsWS[i]);
		newMSJointTrs = glm::scale(newMSJointTrs, entityScale);
		GetSkeleton()->GetJointPtr(ik.m_JointIDs[i])->m_ModelSpaceTransform = newMSJointTrs;
	}
}
