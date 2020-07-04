#include "SkeletonUtils.h"

namespace Chroma
{

	namespace SkeletonUtils
	{
		std::vector<Joint*> GetInbetweenJoints(Skeleton* skeleton, unsigned int const& startJointID, unsigned int const& endJointID)
		{		
			// create joint vector
			std::vector<Joint*> inbetweenJoints;
		
			// add first joint and iter over children until last joint found
			inbetweenJoints.push_back(skeleton->GetJointPtr(startJointID));

			// check whether valid joint chain
			if (skeleton->GetJointPtr(startJointID)->m_ChildJointIDs.size() == 0)
			{
				return inbetweenJoints;
			}

			// collect first child an iter until last child found
			int childJointID = skeleton->GetJointPtr(startJointID)->m_ChildJointIDs[0];
			while (childJointID != endJointID)
			{
				inbetweenJoints.push_back(skeleton->GetJointPtr(childJointID));
				childJointID = skeleton->GetJointPtr(childJointID)->m_ChildJointIDs[0];
			}

			inbetweenJoints.push_back(skeleton->GetJointPtr(endJointID));

			return inbetweenJoints;
		}
		std::vector<unsigned int> GetInbetweenJointIDs(Skeleton* skeleton, unsigned int const& startJointID, unsigned int const& endJointID)
		{
			// create joint vector
			std::vector<unsigned int> inbetweenJoints;

			// add first joint and iter over children until last joint found
			inbetweenJoints.push_back(startJointID);

			// check whether valid joint chain
			if (skeleton->GetJointPtr(startJointID)->m_ChildJointIDs.size() == 0)
			{
				return inbetweenJoints;
			}

			// collect first child an iter until last child found
			int childJointID = skeleton->GetJointPtr(startJointID)->m_ChildJointIDs[0];
			while (childJointID != endJointID)
			{
				inbetweenJoints.push_back(childJointID);
				childJointID = skeleton->GetJointPtr(childJointID)->m_ChildJointIDs[0];
			}

			inbetweenJoints.push_back(endJointID);

			return inbetweenJoints;
		}
		void InitIKConstraint(Skeleton* skeleton, IKConstraint& ikConstraint)
		{
			// Collect chain joint IDs
			ikConstraint.m_JointIDs = GetInbetweenJointIDs(skeleton, ikConstraint.m_RootJointID, ikConstraint.m_EffectorJointID);

			// LENGTH
			// Chain length attrs
			float chainLength{ 0.0 };

			// collect distance for first joint
			ikConstraint.m_JointDistances.push_back(0.0);

			// iterate through remaining joints collecting distances and adding overall chain length
			for (int i = 1; i < ikConstraint.m_JointIDs.size() ; i++)
			{
				glm::vec3 currentPos = Chroma::Math::GetTranslation(skeleton->GetJointPtr(ikConstraint.m_JointIDs[i])->m_ModelSpaceTransform);
				glm::vec3 parentPos = Chroma::Math::GetTranslation(skeleton->GetJointPtr(ikConstraint.m_JointIDs[i-1])->m_ModelSpaceTransform);

				float distToParent = glm::distance(currentPos, parentPos);
				ikConstraint.m_JointDistances.push_back(distToParent);
				chainLength += distToParent;
			}		

			// set final chain length
			ikConstraint.m_ChainLength = chainLength;

			// ROTATIONS
			// iterate through joints collecting rotations and vectors
			for (int i = 0; i < ikConstraint.m_JointIDs.size(); i++)
			{
				glm::vec3 currentPos, childPos;
				// Bind Vectors
				if (i != ikConstraint.m_JointIDs.size()-1) // all but the last joint will have a bind vector
				{
					currentPos = Chroma::Math::GetTranslation(skeleton->GetJointPtr(ikConstraint.m_JointIDs[i])->m_ModelSpaceTransform);
					childPos = Chroma::Math::GetTranslation(skeleton->GetJointPtr(ikConstraint.m_JointIDs[i + 1])->m_ModelSpaceTransform);
				}
				else // direction to target
				{
					currentPos = Chroma::Math::GetTranslation(skeleton->GetJointPtr(ikConstraint.m_JointIDs[i])->m_ModelSpaceTransform);
					childPos = ikConstraint.m_EffectorWorldPos;
				}
				ikConstraint.m_BindVectors.push_back(glm::normalize(childPos - currentPos));

				// Orientation
				ikConstraint.m_BindOrientations.push_back(Chroma::Math::GetQuatRotation(skeleton->GetJointPtr(ikConstraint.m_JointIDs[i])->m_ModelBindTransform));
			}
		}
	}
}