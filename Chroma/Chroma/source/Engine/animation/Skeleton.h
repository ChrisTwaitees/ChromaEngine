#ifndef _CHROMA_SKELETON_
#define _CHROMA_SKELETON_

//common
#include <common/PrecompiledHeader.h>

// chroma
#include <shader/Shader.h>
#include <uid/UID.h>


struct Constraint
{
	enum Type{IK, Aim, Orient, Parent, Null};
	std::string m_Name{ "" };
	int m_RootJointID{ -99 };
	int m_EffectorJointID{ -99 };
	Type m_Type = Null;
};

struct IKConstraint : public Constraint
{
	Type m_Type = IK;
	// effector
	glm::vec3 m_EffectorWorldPos{ 0.0 };
	glm::quat m_EffectorWorldOrient{glm::quat()};

	// joints
	std::vector<unsigned int> m_JointIDs;
	// length
	float m_ChainLength;
	std::vector<float> m_JointDistances;
	// rotations
	std::vector<glm::quat> m_BindOrientations;
	std::vector<glm::vec3> m_BindVectors;
};


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
public:
	// Joints
	void AddJoint(Joint& newJoint);
	void SetRootJoint(Joint& newRootJoint) { m_RootJointID = newRootJoint.m_ID; };
	void SetRootJointID(int const& newRootJointID) { m_RootJointID = newRootJointID; };

	int GetNumJoints() const { return (int)m_Joints.size(); };
	std::map<std::pair<int, std::string>, Joint> GetIndexedNamedJoints() const { return m_Joints; };
	std::map<std::string, Joint*> GetNamedJoints();
	std::map<int, Joint*> GetIndexedJoints();
	std::vector<Joint*> GetJoints();

	int GetJointID(std::string const& jointName) const;
	std::string GetJointName(int const& jointID) const;

	glm::mat4 GetJointTransform(std::string const& jointName) const;
	glm::mat4 GetJointTransform(int const& jointID) const;
	glm::mat4 GetRootTransform() { return BuildRootTransform(); };
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

	// Constraints
	// IK
	inline std::map<std::string, IKConstraint>& GetIKConstraints() { return m_IKConstraints; };
	void AddIKConstraint(IKConstraint& newConstraint);
	
	// Skeleton
	void InitializeSkeleton();
	void SetToBindPose();
	void DebugDrawSkeleton();
	void DebugDrawIKs();
	void Destroy();

	// Functions
	void SetParentComponentUID(UID const& parentComponentUID) { m_ParentComponentUID = parentComponentUID; };
	void SetJointUniforms(Shader& skinnedShader);

	Skeleton();
	~Skeleton();

private:
	// Joints
	int m_RootJointID{ 0 };
	glm::mat4 m_RootTransform{ glm::mat4(1.0) };
	std::map<std::pair<int, std::string>, Joint> m_Joints;

	glm::mat4 BuildRootTransform();
	void UpdateSkeletonRootTransform();

	void CalculateLocalBindOffset(int const& jointID, glm::mat4 const& parentTransform);
	void TransformJointAndChildren(int const& jointID, glm::mat4 const& transform);

	// Components
	UID m_ParentComponentUID;

	// Constraints
	std::map<std::string, IKConstraint> m_IKConstraints;

	// Debug
	void DebugWalkChildJoints(Joint const& currentJoint);

};

#endif