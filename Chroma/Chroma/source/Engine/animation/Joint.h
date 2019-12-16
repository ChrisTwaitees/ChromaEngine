#ifndef _CHROMA_JOINT_
#define _CHROMA_JOINT_

// stl
#include <string>
#include <vector>
#include <iostream>

// thirdparty
#include <glm/glm.hpp>

// chroma

class Joint
{
	// Joint Data
	int m_ID;
	std::string m_Name;
	glm::mat4 m_LocalBindTransform;
	glm::mat4 m_ModelInverseBindTransform;

	glm::mat4 m_AnimatedTransform;
	std::vector<Joint> m_ChildJoints;

public:
	// Accessors
	void SetID(int JointID) { m_ID = JointID; };
	void SetName(std::string Name) { m_Name = Name; };
	void SetLocalBindTransform(glm::mat4 newOffsetMatrix) { m_LocalBindTransform = newOffsetMatrix; };
	void SetAnimatedTransform(glm::mat4 newAnimatedTransform) { m_AnimatedTransform = newAnimatedTransform; };
	void SetModelInverseBindTransform(glm::mat4 newInverseModelBindTransform) { m_ModelInverseBindTransform = newInverseModelBindTransform; };

	void AddChildJoint(Joint newChildJoint) { m_ChildJoints.push_back(newChildJoint); };

	int GetID() const { return m_ID; };
	std::string GetName() const { return m_Name; };
	glm::mat4 GetLocalBindTransform() const { return m_LocalBindTransform; };
	glm::mat4 GetAnimatedTransform() const { return m_AnimatedTransform; };
	std::vector<Joint> GetChildJoints() const { return m_ChildJoints; };
	
	// FUnctions
	void CalculateInverseModelBindTransforms(glm::mat4 parentModelBindTransform);

	Joint();
	~Joint();
};

#endif