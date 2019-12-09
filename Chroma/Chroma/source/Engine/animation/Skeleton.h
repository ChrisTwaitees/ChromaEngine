#ifndef _CHROMA_SKELETON_
#define _CHROMA_SKELETON_

#include <vector>

#include <animation/Joint.h>

class Skeleton
{
	std::vector<Joint> m_joints;

public:
	void addJoint(Joint newJoint) { m_joints.push_back(newJoint); };

	Skeleton();
	~Skeleton();
};

#endif