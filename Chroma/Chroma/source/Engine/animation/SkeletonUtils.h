#ifndef CHROMA_SKELETON_UTILS_H
#define CHROMA_SKELETON_UTILS_H

#include <common/PrecompiledHeader.h>
#include <animation/Skeleton.h>

namespace SkeletonUtils
{
	std::vector<Joint*> GetInbetweenJoints(Skeleton* skeleton, unsigned int const& startJointID, unsigned int const& endJointID);
	std::vector<unsigned int> GetInbetweenJointIDs(Skeleton* skeleton, unsigned int const& startJointID, unsigned int const& endJointID);
	void InitIKConstraint(Skeleton* skeleton, IKConstraint& ikConstraint);
}


#endif //CHROMA_SKELETON_UTILS_H