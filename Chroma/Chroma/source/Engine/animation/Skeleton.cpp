#include "Skeleton.h"
#include <entity/IEntity.h>


void Skeleton::InitializeSkeleton()
{
	// calculate local bind offset relative to parent joint
	CalculateJointLocalBindOffsetTransforms();
}

void Skeleton::AddJoint(Joint& newJoint)
{
	m_Joints[make_pair(newJoint.GetID(), newJoint.GetName())] = newJoint;
}

void Skeleton::SetGlobalTransform(glm::mat4 const& newGlobalTransform)
{
	m_WorldTransform = newGlobalTransform;
	m_WorldTransformInverse = glm::inverse(newGlobalTransform);
}

void Skeleton::SetToBindPose()
{
	for (auto& IDNameJoint : m_Joints)
	{
		IDNameJoint.second.SetModelSpaceTransform( IDNameJoint.second.GetModelBindTransform());
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
}

glm::mat4 Skeleton::GetJointTransform(std::string const& jointName) const
{
	for (auto& IDNameJoint : m_Joints)
	{
		if (IDNameJoint.first.second == jointName)
		{
			return IDNameJoint.second.GetModelSpaceTransform();
		}
	}
	CHROMA_ERROR("SKELETON ERROR :: JOINT COULD NOT BE FOUND: {0}", jointName);
}

glm::mat4 Skeleton::GetJointTransform(int const& jointID) const
{
	for (auto& IDNameJoint : m_Joints)
	{
		if (IDNameJoint.first.first == jointID)
		{
			return IDNameJoint.second.GetModelSpaceTransform();
		}
	}
	CHROMA_ERROR("SKELETON ERROR :: JOINT COULD NOT BE FOUND: {0}", jointID);
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

void Skeleton::DebugDraw(DebugBuffer* debugBuffer)
{
	// Loop through Skeleton drawing to debug buffer
	DebugWalkChildJoints(GetRootJoint(), debugBuffer);
}

glm::mat4 Skeleton::BuildRootTransform()
{
	// Build rootTransform Matrix
	glm::mat4 rootTransform = glm::translate(m_IdentityMatrix, m_Translation);
	rootTransform = glm::toMat4(m_Rotation) * rootTransform;
	return glm::scale(rootTransform, glm::vec3(m_Scale));
}

void Skeleton::DebugWalkChildJoints(Joint const& currentJoint, DebugBuffer* const &debugBuffer)
{
	// Debug Draw Skelton
	glm::mat4 worldJointTransform = GetRootTransform() * currentJoint.GetModelSpaceTransform();
	// Coordinates
	debugBuffer->DrawOverlayCoordinates(worldJointTransform, 3.5);
	// Joint
	glm::vec3 startPos = GLMGetTranslation(GetRootTransform() * currentJoint.GetModelSpaceTransform());

	for (int const& childID : currentJoint.GetChildJointIDs())
	{
		glm::vec3 endPos = GLMGetTranslation(GetRootTransform() * GetJoint(childID).GetModelSpaceTransform());
		if(currentJoint.GetID() == m_RootJointID)
			debugBuffer->DrawOverlayJoint(startPos, endPos, worldJointTransform, 1.0, glm::vec3(1.0, 0.0, 0.0));
		else if (childID == GetJointID("mixamorig:Head"))
			debugBuffer->DrawOverlayJoint(startPos, endPos, worldJointTransform, 1.0, glm::vec3(1.0, 1.0, 0.0));
		else
			debugBuffer->DrawOverlayJoint(startPos, endPos, worldJointTransform, 1.0);

		DebugWalkChildJoints(GetJoint(childID), debugBuffer);
	}
}

void Skeleton::SetJointUniforms(Shader& skinnedShader)
{
	/*std::vector<glm::mat4> Transforms;
	boneTransform((float)currentTime, Transforms);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "gBones"), (GLsizei)Transforms.size(), GL_FALSE, glm::value_ptr(Transforms[0]));*/
	// Render Pipeline Entry point, setting shader's Joint Matrices
	for (auto const& IDNameJoint : m_Joints)
	{
		glm::mat4 WorldSpaceOffset = GetRootTransform() * IDNameJoint.second.GetModelSpaceTransform() * IDNameJoint.second.GetModelInverseBindTransform();
		std::string jntUniformName = "aJoints[" + std::to_string(IDNameJoint.first.first) + "]";
		skinnedShader.setUniform(jntUniformName, WorldSpaceOffset);
	}
}

void Skeleton::UpdateSkeletonRootTransform()
{
	// Apply to root, traversing down chain
	m_RootTransform = BuildRootTransform();
}

void Skeleton::TransformJointAndChildren(int const& jointID, glm::mat4 const& transform)
{
	// Recursive applying offset from Model Bind Transform
	glm::mat4 updatedTransform{ transform * GetJointPtr(jointID)->GetModelBindTransform()};
	GetJointPtr(jointID)->SetModelSpaceTransform(updatedTransform);

	for (int const& childID : GetJointPtr(jointID)->GetChildJointIDs())
	{
		TransformJointAndChildren(childID, transform);
	}
}

void Skeleton::CalculateJointLocalBindOffsetTransforms()
{
	// Init Skeleton, calculating local joint offset to parent
	for (auto& IDNameJoint : m_Joints)
	{
		if (IDNameJoint.second.GetParentJointID() != -1) // root joint has no parent
		{
			glm::mat4 parentModelBindTransform = GetJointPtr(IDNameJoint.second.GetParentJointID())->GetModelBindTransform();
			glm::mat4 currentInverseModelBindTransform = IDNameJoint.second.GetModelInverseBindTransform();
			glm::mat4 localModelBindTransform = glm::inverse(parentModelBindTransform * currentInverseModelBindTransform);
			IDNameJoint.second.SetLocalBindTransform(localModelBindTransform);
		}
		else
		{
			GetJointPtr(IDNameJoint.first.first)->SetLocalBindTransform(glm::mat4(1.0));
		}
	}
}


Skeleton::Skeleton()
{
}


Skeleton::~Skeleton()
{
}
