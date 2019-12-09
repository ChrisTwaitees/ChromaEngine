#ifndef _CHROMA_SKELETON_
#define _CHROMA_SKELETON_

// stl
#include <vector>
// chroma
#include <animation/Joint.h>

class Skeleton
{
	std::vector<Joint> m_Joints;

public:
	// Accessors
	void AddJoint(Joint newJoint) { m_Joints.push_back(newJoint); };

	Skeleton();
	~Skeleton();
};

#endif