#ifndef _CHROMA_SKELETON_
#define _CHROMA_SKELETON_

// stl
#include <vector>
#include <map>


// chroma
#include <animation/Joint.h>

class Skeleton
{
	glm::mat4 m_GlobalTransform{ 1.0 };
	glm::mat4 m_GlobalTransformInverse{ 1.0 };
	Joint m_RootJoint;
	std::map<std::string, Joint> m_NamedJoints;
	std::map<int, Joint> m_IndexedJoints;

public:
	// Accessors
	void AddJoint(Joint const& newJoint);
	void SetGlobalTransform(glm::mat4 const& newGlobalTransform) ;

	int GetNumJoints() const { return m_IndexedJoints.size(); };
	Joint GetJoint(int& index);
	Joint GetJoint(std::string const& jointName);

	// Functions
	void CalculateJointBindTransforms();

	Skeleton();
	~Skeleton();
};

#endif