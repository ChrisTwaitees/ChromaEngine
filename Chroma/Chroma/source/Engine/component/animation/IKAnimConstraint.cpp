#include "IKAnimConstraint.h"
#include <entity/Entity.h>
#include <animation/SkeletonUtils.h>
#include <animation/Animator.h>
#include <glm/gtx/quaternion.hpp>

namespace Chroma
{
	void IKAnimConstraint::Init()
	{
		// Set Type
		m_Type = Type::Component::kIKAnimationConstraintComponent;

		CMPNT_INITIALIZED
	}

	void IKAnimConstraint::OnUpdate()
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
		CMPNT_DESTROYED
	}

	void IKAnimConstraint::Serialize(ISerializer*& serializer)
	{
		CMPNT_SERIALIZE_BEGIN
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
		// positions
		std::vector<glm::vec3> jntPositionsWS;
		// entity scale
		glm::vec3 entityScale = GetParentEntity()->GetScale();
		// rotation
		std::vector<glm::quat> jntBindOrientationsWS;
		glm::quat rootRotation = GetJointOrientationWS(ik.m_RootJointID);

		// get positions and orientations WS
		for (int i = 0; i < ik.m_JointIDs.size(); i++)
		{
			jntPositionsWS.push_back(Math::GetTranslation(GetSkeleton()->GetRootTransform() * GetSkeleton()->GetJointPtr(ik.m_JointIDs[i])->m_ModelSpaceTransform));
			jntBindOrientationsWS.push_back(Math::GetQuatRotation(GetSkeleton()->GetRootTransform()) * ik.m_BindOrientations[i]);
		}

		// POSITIONS
		// root to effector
		float distToEffector = glm::distance(jntPositionsWS[0], ik.m_EffectorWorldPos);
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

		// POLES


		// ROTATIONS
		// set positions and rotations
		glm::mat4 animatedJntTransform(1.0);
		glm::mat4 finalJntTransform(1.0);
		glm::quat newOrient = glm::quat();
		for (int i = 0; i < ik.m_JointIDs.size(); i++)
		{
			// rotation
			if (i == ik.m_JointIDs.size() - 1) // do not apply to last bone
				newOrient = glm::inverse(glm::inverse(ik.m_EffectorWorldOrient) * glm::inverse(ik.m_BindOrientations[i])) * rootRotation;
			else
				newOrient = Chroma::Math::FromToRotation(ik.m_BindVectors[i], jntPositionsWS[i+1] - jntPositionsWS[i]) * glm::inverse(jntBindOrientationsWS[i]);

			// build transform matrix
			finalJntTransform = glm::inverse(GetSkeleton()->GetRootTransform()) * glm::mat4(1.0);
			finalJntTransform = glm::translate(finalJntTransform, jntPositionsWS[i]); // translation 
			finalJntTransform *=   glm::toMat4(newOrient); // orientation
			finalJntTransform = glm::scale(finalJntTransform, entityScale); // scale

			// grab animated transform and set new transform
			animatedJntTransform = GetSkeleton()->GetJointPtr(ik.m_JointIDs[i])->m_ModelSpaceTransform;
			GetSkeleton()->GetJointPtr(ik.m_JointIDs[i])->m_ModelSpaceTransform = finalJntTransform;
		}

		// Propogate Animation
		// Apply final transform to all children joints
		if (GetSkeleton()->GetJointPtr(ik.m_JointIDs.back())->m_ChildJointIDs.size() > 0)
			GetSkeleton()->TransformJointMSAndChildren(GetSkeleton()->GetJointPtr(ik.m_JointIDs.back())->m_ChildJointIDs[0], finalJntTransform * glm::inverse(animatedJntTransform));
	}



	glm::quat IKAnimConstraint::GetJointOrientationWS(unsigned int const& jointID)
	{
		return Math::GetQuatRotation(GetSkeleton()->GetRootTransform() * GetSkeleton()->GetJointPtr(jointID)->m_ModelSpaceTransform);
	}
}
