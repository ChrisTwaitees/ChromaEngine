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
	Joint* m_RootJoint{new Joint};
	std::map<std::pair<int, std::string>, Joint*> m_Joints;

public:
	// Accessors
	void AddJoint(Joint*& newJoint);
	void SetGlobalTransform(glm::mat4 const& newGlobalTransform) ;
	void SetRootJoint(Joint* newRootJoint) { m_RootJoint = newRootJoint; };

	int GetNumJoints() const { return m_Joints.size(); };
	std::map<std::pair<int, std::string>, Joint*> GetIndexedNamedJoints() const { return m_Joints; };
	std::map<std::string, Joint*> GetNamedJoints() const ;
	std::map<int, Joint*> GetIndexedJoints() const ;
	std::vector<Joint*> GetJoints() const;

	Joint* GetJoint(int const& index);
	Joint* GetJoint(std::string const& jointName);

	bool GetJointExists(int const& index) const;
	bool GetJointExists(std::string const& jointName) const;

	// Functions
	void CalculateJointBindTransforms();

	Skeleton();
	~Skeleton();
};

#endif