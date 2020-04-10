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
}