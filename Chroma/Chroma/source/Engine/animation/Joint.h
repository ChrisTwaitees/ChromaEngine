#ifndef _CHROMA_JOINT_
#define _CHROMA_JOINT_

// stl
#include <string>
#include <vector>
#include <iostream>

// thirdparty
#include <glm/glm.hpp>

// chroma
#include <math/Math.h>

class Joint
{
	// Identifiers
	int m_ID{0};
	std::string m_Name;

	// Transforms
	glm::mat4 m_ModelBindTransform{ 1.0 }; // Model - space bind transform, relative to model root
	glm::mat4 m_ModelInverseBindTransform{ 1.0 }; // Inverse of Model Bind Transform
	glm::mat4 m_LocalBindOffsetTransform{ 1.0 }; // Joint - space bind Transform, relative to parent modelBindTransform

	glm::mat4 m_ModelSpaceTransform{ 1.0 }; // Passed to WorldSpace Transform for Shader

	// Parent/ Child Joint IDs
	std::vector<int> m_ChildJointIDs; 
	int m_ParentJointID{ 0 };

public:
	// Accessors Setters
	void SetID(int JointID) { m_ID = JointID; };
	void SetName(std::string Name) { m_Name = Name; };
	
	void SetModelBindTransform(glm::mat4 newModelBindTransform) { m_ModelBindTransform = newModelBindTransform; };
	void SetLocalBindTransform(glm::mat4 newOffsetMatrix) { m_LocalBindOffsetTransform = newOffsetMatrix; };
	void SetModelInverseBindTransform(glm::mat4 newInverseModelBindTransform) { m_ModelInverseBindTransform = newInverseModelBindTransform; };
	
	void SetModelSpaceTransform(glm::mat4 newFinalTransform) { m_ModelSpaceTransform = newFinalTransform; };

	void AddChildJointID(int const&  newChildJoint) { m_ChildJointIDs.push_back(newChildJoint); };
	void SetChildJointIDs(std::vector<int> const& newChildJoints) { m_ChildJointIDs = newChildJoints; };
	void SetParentJointID(int const& newParentJoint) { m_ParentJointID = newParentJoint; };

	// Accessors Setters
	int GetID() const { return m_ID; };
	std::string GetName() const { return m_Name; };

	glm::mat4 GetModelBindTransform() const { return m_ModelBindTransform; };
	glm::mat4 GetModelInverseBindTransform() const { return m_ModelInverseBindTransform; };
	glm::mat4 GetLocalBindOffsetTransform() const { return m_LocalBindOffsetTransform; };

	glm::mat4 GetModelSpaceTransform() const { return m_ModelSpaceTransform; };

	std::vector<int> GetChildJointIDs() const { return m_ChildJointIDs; };
	int GetParentJointID() const { return m_ParentJointID; };
	
	
	Joint();
	~Joint();
};

#endif