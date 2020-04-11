#include "SkeletonUtils.h"


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

		// Chain length attrs
		float chainLength{ 0.0 };

		// collect distance for first joint
		ikConstraint.m_JointDistances.push_back(0.0);

		// iterate through remaining joints collecting distances and adding overall chain length
		for (int i = 1; i < ikConstraint.m_JointIDs.size() ; i++)
		{
			glm::vec3 currentPos = Chroma::Math::GetTranslation(skeleton->GetJointPtr(ikConstraint.m_JointIDs[i])->m_ModelSpaceTransform);
			glm::vec3 parentPos = Chroma::Math::GetTranslation(skeleton->GetJointPtr(ikConstraint.m_JointIDs[i-1])->m_ModelSpaceTransform);

			// collect distances 
			ikConstraint.m_JointDistances.push_back((currentPos - parentPos).length());
			chainLength += (currentPos - parentPos).length();
		}		

		// set final chain length
		ikConstraint.m_ChainLength = chainLength;
	}
}