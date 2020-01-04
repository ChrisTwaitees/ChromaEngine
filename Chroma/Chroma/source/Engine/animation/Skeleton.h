#ifndef _CHROMA_SKELETON_
#define _CHROMA_SKELETON_

// stl
#include <vector>
#include <map>

// thirdparty
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// chroma
#include <buffer/DebugBuffer.h>


struct Joint
{
	// Identifiers
	int m_ID{ 0 };
	std::string m_Name;

	// Transforms
	glm::mat4 m_ModelBindTransform{ 1.0 }; // Model - space bind transform, relative to model root
	glm::mat4 m_ModelInverseBindTransform{ 1.0 }; // Inverse of Model Bind Transform
	glm::mat4 m_LocalBindOffsetTransform{ 1.0 }; // Joint - space bind Transform, relative to parent modelBindTransform

	glm::mat4 m_ModelSpaceTransform{ 1.0 }; // Passed to WorldSpace Transform for Shader

	// Parent/ Child Joint IDs
	std::vector<int> m_ChildJointIDs;
	int m_ParentJointID{ 0 };
};



class Skeleton
{
	glm::mat4 m_IdentityMatrix{ 1.0 };
	glm::mat4 m_WorldTransform{ 1.0 };
	glm::mat4 m_WorldTransformInverse{ 1.0 };
	int m_RootJointID{ 0 };
	std::map<std::pair<int, std::string>, Joint> m_Joints;

	float m_Scale{ 1.0f };
	glm::vec3 m_Translation{ 1.0f };
	glm::quat m_Rotation{ glm::quat() };
	glm::mat4 m_RootTransform{ glm::mat4(1.0) };

	// Functions
	glm::mat4 BuildRootTransform();
	void DebugWalkChildJoints(Joint const& currentJoint, DebugBuffer* const& debugBuffer);
	void UpdateSkeletonRootTransform();

public:
	// Accessors
	void AddJoint(Joint& newJoint);
	void SetGlobalTransform(glm::mat4 const& newGlobalTransform) ;
	void SetRootJoint(Joint& newRootJoint) { m_RootJointID = newRootJoint.m_ID; };
	void SetRootJointID(int const& newRootJointID) { m_RootJointID = newRootJointID; };

	void SetScale(float const& newScale) { m_Scale = newScale; UpdateSkeletonRootTransform(); };
	void SetTranslation(glm::vec3 const& newTranslation) { m_Translation = newTranslation; UpdateSkeletonRootTransform(); };
	void SetRotation(glm::quat const& newRotation) { m_Rotation = newRotation; UpdateSkeletonRootTransform(); };

	void SetToBindPose();

	int GetNumJoints() const { return m_Joints.size(); };
	std::map<std::pair<int, std::string>, Joint> GetIndexedNamedJoints() const { return m_Joints; };
	std::map<std::string, Joint*> GetNamedJoints();
	std::map<int, Joint*> GetIndexedJoints();
	std::vector<Joint*> GetJoints();

	int GetJointID(std::string const& jointName) const;
	std::string GetJointName(int const& jointID) const;

	glm::mat4 GetJointTransform(std::string const& jointName) const;
	glm::mat4 GetJointTransform(int const& jointID) const;
	glm::mat4 GetRootTransform() const { return m_RootTransform; };
	unsigned int GetRootJointID() const { return m_RootJointID; };

	Joint GetJoint(int const& index);
	Joint* GetJointPtr(int const& index);
	Joint GetJoint(std::string const& jointName);
	Joint* GetJointPtr(std::string const& jointName);
	Joint GetJointPartialName(std::string const& jointName);
	Joint* GetJointPtrPartialName(std::string const& jointName);
	Joint GetRootJoint() { return GetJoint(m_RootJointID); };

	bool GetJointExists(int const& index) const;
	bool GetJointExists(std::string const& jointName) const;

	// Functions
	void SetJointUniforms(Shader& skinnedShader);
	void CalculateJointLocalBindOffsetTransforms();
	void TransformJointAndChildren(int const& jointID, glm::mat4 const& transform);
	void DebugDraw(DebugBuffer* debugBuffer);
	void InitializeSkeleton();

	Skeleton();
	~Skeleton();
};

#endif