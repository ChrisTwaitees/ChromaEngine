#include "Skeleton.h"
#include <entity/IEntity.h>
#include <component/MeshComponent.h>
#include <render/Render.h>
#include <animation/SkeletonUtils.h>



namespace Chroma
{

	void Skeleton::AddIKConstraint(IKConstraint& newConstraint)
	{
		m_IKConstraints.insert(std::make_pair(newConstraint.m_Name, newConstraint));
	}

	void Skeleton::InitializeSkeleton()
	{
		// calculate local bind m_Offset relative to parent joint
		CalculateLocalBindOffset(GetRootJointID(), glm::mat4{ 1.0 });

		// collect jointIDs for constraints
		for (auto& ik : m_IKConstraints)
			Chroma::SkeletonUtils::InitIKConstraint(this, ik.second);
	}

	void Skeleton::Destroy()
	{
		m_Joints.clear();
		CHROMA_INFO("Skeleton Destroyed.");
	}

	void Skeleton::AddJoint(Joint& newJoint)
	{
		m_Joints[make_pair(newJoint.m_ID, newJoint.m_Name)] = newJoint;
	}

	void Skeleton::SetToBindPose()
	{
		for (auto& IDNameJoint : m_Joints)
		{
			IDNameJoint.second.m_ModelSpaceTransform = IDNameJoint.second.m_ModelBindTransform;
		}

	}

	std::map<std::string, Joint*> Skeleton::GetNamedJoints()
	{
		std::map<std::string, Joint*> newJointNameMap;
		for (auto& IDNameJoint : m_Joints)
		{
			newJointNameMap[IDNameJoint.first.second] = &IDNameJoint.second;
		}
		return newJointNameMap;
	}

	std::map<int, Joint*> Skeleton::GetIndexedJoints()
	{
		std::map<int, Joint*> newJointIDMap;
		for (auto& IDNameJoint : m_Joints)
		{
			newJointIDMap[IDNameJoint.first.first] = &IDNameJoint.second;
		}
		return newJointIDMap;
	}

	std::vector<Joint*> Skeleton::GetJoints()
	{
		std::vector<Joint*> skeletonJoints;
		for (auto& IDNameJoint : m_Joints)
		{
			skeletonJoints.push_back(&IDNameJoint.second);
		}
		return skeletonJoints;
	}

	int Skeleton::GetJointID(std::string const& jointName) const
	{
		for (auto const& IDNameJoint : m_Joints)
		{
			if (IDNameJoint.first.second == jointName)
			{
				return IDNameJoint.first.first;
			}
		}
		CHROMA_ERROR("SKELETON ERROR :: JOINT ID COULD NOT BE FOUND : {0} ", jointName);
		return -99;
	}

	std::string Skeleton::GetJointName(int const& jointID) const
	{
		for (auto const& IDNameJoint : m_Joints)
		{
			if (IDNameJoint.first.first == jointID)
			{
				return IDNameJoint.first.second;
			}
		}
		CHROMA_ERROR("SKELETON ERROR :: JOINT Name COULD NOT BE FOUND : {0} ", jointID);
		return "";
	}

	glm::mat4 Skeleton::GetJointTransform(std::string const& jointName) const
	{
		for (auto& IDNameJoint : m_Joints)
		{
			if (IDNameJoint.first.second == jointName)
			{
				return IDNameJoint.second.m_ModelSpaceTransform;
			}
		}
		CHROMA_ERROR("SKELETON ERROR :: JOINT COULD NOT BE FOUND: {0}", jointName);
		return glm::mat4(1.0);
	}

	glm::mat4 Skeleton::GetJointTransform(int const& jointID) const
	{
		for (auto& IDNameJoint : m_Joints)
		{
			if (IDNameJoint.first.first == jointID)
			{
				return IDNameJoint.second.m_ModelSpaceTransform;
			}
		}
		CHROMA_ERROR("SKELETON ERROR :: JOINT COULD NOT BE FOUND: {0}", jointID);
		return glm::mat4(1.0);
	}

	void Skeleton::TransformJointMSAndChildren(int const& jointID, glm::mat4 const& transform)
	{
		// Recursive applying transform from Model Bind Transform
		glm::mat4 updatedTransform{ transform * GetJointPtr(jointID)->m_ModelSpaceTransform };
		GetJointPtr(jointID)->m_ModelSpaceTransform = updatedTransform;

		for (int const& childID : GetJointPtr(jointID)->m_ChildJointIDs)
		{
			TransformJointMSAndChildren(childID, transform);
		}
	}

	Joint Skeleton::GetJoint(int const& index)
	{
		for (auto & IDNameJoint : m_Joints)
		{
			if (IDNameJoint.first.first == index)
			{
				return IDNameJoint.second;
			}
		}
		CHROMA_ERROR("SKELETON ERROR :: JOINT COULD NOT BE FOUND: {0}", index);
		return Joint();
	}

	Joint Skeleton::GetJoint(std::string const& jointName) 
	{
		for (auto& IDNameJoint : m_Joints)
		{
			if (IDNameJoint.first.second == jointName)
			{
				return IDNameJoint.second;
			}
		}
		CHROMA_ERROR("SKELETON ERROR :: JOINT COULD NOT BE FOUND: {0}", jointName);
		return Joint();
	}

	Joint* Skeleton::GetJointPtr(int const& index)
	{
		for (auto& IDNameJoint : m_Joints)
		{
			if (IDNameJoint.first.first == index)
			{
				return &IDNameJoint.second;
			}
		}
		return nullptr;
	}

	Joint* Skeleton::GetJointPtr(std::string const& jointName)
	{
		for (auto& IDNameJoint : m_Joints)
		{
			if (IDNameJoint.first.second == jointName)
			{
				return &IDNameJoint.second;
			}
		}
		return nullptr;

	}

	Joint Skeleton::GetJointPartialName(std::string const& jointName)
	{
		for (auto& IDNameJoint : m_Joints)
		{
			if (jointName.find(IDNameJoint.first.second) != std::string::npos)
			{
				return IDNameJoint.second;
			}
		}
		CHROMA_ERROR("SKELETON ERROR :: JOINT COULD NOT BE FOUND: {0}", jointName);
		return Joint();
	}

	Joint* Skeleton::GetJointPtrPartialName(std::string const& jointName)
	{
		for (auto& IDNameJoint : m_Joints)
		{
			if (jointName.find(IDNameJoint.first.second) != std::string::npos)
			{
				return &IDNameJoint.second;
			}
		}
		return nullptr;
	}

	bool Skeleton::GetJointExists(int const& index) const
	{
		for (auto const& IDNameJoint : m_Joints)
		{
			if (IDNameJoint.first.first == index)
			{
				return true;
			}
		}
		CHROMA_ERROR("SKELETON ERROR :: JOINT COULD NOT BE FOUND: {0}", index);
		return false;
	}

	bool Skeleton::GetJointExists(std::string const& jointName) const
	{
		for (auto const& IDNameJoint : m_Joints)
		{
			if (IDNameJoint.first.second == jointName)
			{
				return true;
			}
		}
		// could not find
		return false;
	}

	void Skeleton::DebugDrawSkeleton()
	{
		// Loop through Skeleton drawing to debug buffer
		DebugWalkChildJoints(GetRootJoint());
	}

	void Skeleton::DebugDrawIKs()
	{
		glm::mat4 jointTrs{ 1.0 };
		glm::vec3 root{ 0.0 };
		glm::vec3 end{ 0.0 };
		glm::vec3 jointColor{ 0.019, 0.776, 1 };

		for (std::pair<std::string, IKConstraint> const& ik : m_IKConstraints)
		{
			// Affected Joints
			for (int i = 0; i < ik.second.m_JointIDs.size(); i++)
			{

				//first joint
				jointColor = i == 0 ? glm::vec3(1, 0.019, 0.933) : glm::vec3(0.019, 0.776, 1);

				if (i != ik.second.m_JointIDs.size()-1) // all but last
				{
					jointTrs = GetRootTransform() * GetJointPtr(ik.second.m_JointIDs[i])->m_ModelSpaceTransform;
					root = GLMGetTranslation(GetRootTransform() * GetJointPtr(ik.second.m_JointIDs[i])->m_ModelSpaceTransform);
					end = GLMGetTranslation(GetRootTransform() * GetJointPtr(ik.second.m_JointIDs[i + 1])->m_ModelSpaceTransform);
					// Joint
					Chroma::Render::GetDebugBuffer()->DrawOverlayJoint(root, end, jointTrs, 1.0, jointColor);
					// Coordinates
					Chroma::Render::GetDebugBuffer()->DrawOverlayCoordinates(jointTrs, 2.0);
				}
				else // just draw transform
				{
					jointTrs = GetRootTransform() * GetJointPtr(ik.second.m_JointIDs[i])->m_ModelSpaceTransform;
					// Coordinates
					Chroma::Render::GetDebugBuffer()->DrawOverlayCoordinates(jointTrs, 2.0);
				}

			}

			// Effector Line
			// Root
			root = GLMGetTranslation(GetRootTransform() * GetJointPtr(ik.second.m_RootJointID)->m_ModelSpaceTransform);
			// Effector
			end = ik.second.m_EffectorWorldPos;
			Chroma::Render::GetDebugBuffer()->DrawOverlayLine(root, end, glm::vec3(0.101, 0.541, 1));
			Chroma::Render::GetDebugBuffer()->DrawOverlayCross(end, 1.0, glm::vec3(0.262, 1, 0.019));
		}
	}

	glm::mat4 Skeleton::BuildRootTransform()
	{
		return ((MeshComponent*)Chroma::Scene::GetComponent(m_ParentComponentUID))->GetWorldTransform();
	}

	void Skeleton::DebugWalkChildJoints(Joint const& currentJoint)
	{
		// Debug Draw Skeleton
		glm::mat4 worldJointTransform = GetRootTransform() * currentJoint.m_ModelSpaceTransform;
		// Coordinates
		Chroma::Render::GetDebugBuffer()->DrawOverlayCoordinates(worldJointTransform, 4.5);
		// Joint 
		glm::vec3 startPos = GLMGetTranslation(GetRootTransform() * currentJoint.m_ModelSpaceTransform);
		for (int const& childID : currentJoint.m_ChildJointIDs)
		{
			glm::vec3 endPos = GLMGetTranslation(GetRootTransform() * GetJoint(childID).m_ModelSpaceTransform);

			if(currentJoint.m_ID == m_RootJointID)
				Chroma::Render::GetDebugBuffer()->DrawOverlayJoint(startPos, endPos, worldJointTransform, 1.0, glm::vec3(1.0, 0.0, 0.0));
			else
				Chroma::Render::GetDebugBuffer()->DrawOverlayJoint(startPos, endPos, worldJointTransform, 1.0);

			DebugWalkChildJoints(GetJoint(childID));
		}
	}



	void Skeleton::SetJointUniforms(Shader& skinnedShader)
	{
		for (auto const& IDNameJoint : m_Joints)
		{
			glm::mat4 WorldSpaceOffset =  IDNameJoint.second.m_ModelSpaceTransform * IDNameJoint.second.m_ModelInverseBindTransform;
			std::string jntUniformName = "aJoints[" + std::to_string(IDNameJoint.first.first) + "]";
			skinnedShader.SetUniform(jntUniformName, WorldSpaceOffset);
		}
	}

	void Skeleton::UpdateSkeletonRootTransform()
	{
		// Apply to root, traversing down chain
		m_RootTransform = BuildRootTransform();
	}

	void Skeleton::TransformJointAndChildren(int const& jointID, glm::mat4 const& transform)
	{
		// Recursive applying transform from Model Bind Transform
		glm::mat4 updatedTransform{ transform * GetJointPtr(jointID)->m_ModelBindTransform };
		GetJointPtr(jointID)->m_ModelSpaceTransform = updatedTransform;

		for (int const& childID : GetJointPtr(jointID)->m_ChildJointIDs)
		{
			TransformJointAndChildren(childID, transform);
		}
	}

	void Skeleton::CalculateLocalBindOffset(int const& jointID, glm::mat4 const& parentTransform)
	{
		// Init Skeleton, calculating local joint m_Offset to parent
		glm::mat4 modelBindTransform = GetJointPtr(jointID)->m_ModelBindTransform;
		GetJointPtr(jointID)->m_LocalBindOffsetTransform = glm::inverse(parentTransform * GetJointPtr(jointID)->m_ModelInverseBindTransform);

		for (int const& childJointID : GetJointPtr(jointID)->m_ChildJointIDs)
		{
			CalculateLocalBindOffset(childJointID, modelBindTransform);
		}
	}


	Skeleton::Skeleton()
	{
	}


	Skeleton::~Skeleton()
	{
	}
}
