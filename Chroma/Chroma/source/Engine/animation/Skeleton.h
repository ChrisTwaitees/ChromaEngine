#ifndef _CHROMA_SKELETON_
#define _CHROMA_SKELETON_

// stl
#include <vector>
#include <map>

// chroma
#include <animation/Joint.h>
#include <buffer/DebugBuffer.h>


class Skeleton
{
	glm::mat4 m_GlobalTransform{ 1.0 };
	glm::mat4 m_GlobalTransformInverse{ 1.0 };
	int m_RootJointID{ 0 };
	std::map<std::pair<int, std::string>, Joint> m_Joints;

	// Functions
	void ProcessChildModelBindTransforms(Joint* currentJoint, glm::mat4 const& parentTransform);
	void DebugWalkChildJoints(Joint* currentJoint, DebugBuffer* const& debugBuffer);
public:
	// Accessors
	void AddJoint(Joint& newJoint);
	void SetGlobalTransform(glm::mat4 const& newGlobalTransform) ;
	void SetRootJoint(Joint& newRootJoint) { m_RootJointID = newRootJoint.GetID(); };
	void SetRootJointID(int const& newRootJointID) { m_RootJointID = newRootJointID; };

	int GetNumJoints() const { return m_Joints.size(); };
	std::map<std::pair<int, std::string>, Joint> GetIndexedNamedJoints() const { return m_Joints; };
	std::map<std::string, Joint*> GetNamedJoints();
	std::map<int, Joint*> GetIndexedJoints();
	std::vector<Joint*> GetJoints();

	int GetJointID(std::string const& jointName) const;
	std::string GetJointName(int const& jointID) const;

	Joint GetJoint(int const& index);
	Joint GetJoint(std::string const& jointName);
	Joint GetRootJoint() { return GetJoint(m_RootJointID); };

	bool GetJointExists(int const& index) const;
	bool GetJointExists(std::string const& jointName) const;

	// Functions
	void CalculateJointBindTransforms();
	void DebugDraw(DebugBuffer* debugBuffer);

	Skeleton();
	~Skeleton();
};

#endif