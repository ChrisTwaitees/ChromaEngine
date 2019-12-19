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
	std::cout << "::SKELETON ERROR::" << std::endl;
	std::cout << "JOINT ID COULD NOT BE FOUND WITH NAME : " << jointName << std::endl;
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
	std::cout << "::SKELETON ERROR::" << std::endl;
	std::cout << "JOINT NAME COULD NOT BE FOUND WITH ID : " << jointID << std::endl;
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
	std::cout << "::SKELETON ERROR::" << std::endl;
	std::cout << "JOINT ID : " << index << " COULD NOT BE FOUND. " << std::endl;
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
	std::cout << "::SKELETON ERROR::" << std::endl;
	std::cout << "JOINT NAME : " << jointName << " COULD NOT BE FOUND. " << std::endl;
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

bool Skeleton::GetJointExists(int const& index) const
{
	for (auto const& IDNameJoint : m_Joints)
	{
		if (IDNameJoint.first.first == index)
		{
			return true;
		}
	}
	// could not find
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

void Skeleton::DebugDraw(DebugBuffer* debugBuffer)
{
	// Loop through Skeleton drawing to debug buffer
	DebugWalkChildJoints(GetRootJoint(), debugBuffer);
}

void Skeleton::DebugWalkChildJoints(Joint const& currentJoint, DebugBuffer* const &debugBuffer)
{
	// Debug Draw Skelton
	glm::vec3 startPos = GLMGetTranslation(GetJoint(currentJoint.GetID()).GetFinalTransform());

	for (int const& childID : currentJoint.GetChildJointIDs())
	{
		glm::vec3 endPos = GLMGetTranslation(GetJoint(childID).GetFinalTransform());
		if(currentJoint.GetID() == m_RootJointID)
			debugBuffer->DrawOverlayLine(startPos, endPos, glm::vec3(1.0 , 0.0,  0.0));
		else if (childID == GetJointID("mixamorig:Head"))
			debugBuffer->DrawOverlayLine(startPos, endPos, glm::vec3(1.0, 1.0, 0.0));
		else
			debugBuffer->DrawOverlayLine(startPos, endPos, glm::vec3(1.0));

		DebugWalkChildJoints(GetJoint(childID), debugBuffer);
	}
}


void Skeleton::UpdateSkeletonRootTransform()
{
	// Build rootTransform Matrix
	glm::mat4 rootTransform = glm::translate(m_IdentityMatrix, m_Translation);
	rootTransform = glm::toMat4(m_Rotation) * rootTransform;
	rootTransform = glm::scale(rootTransform, glm::vec3(m_Scale));
	// Apply to root, traversing down chain
	TransformJointAndChildren(m_RootJointID, rootTransform);
}

void Skeleton::TransformJointAndChildren(int const& jointID, glm::mat4 const& transform)
{
	// Recursive applying offset from Model Bind Transform
	glm::mat4 updatedTransform{ transform * GetJointPtr(jointID)->GetModelBindTransform()};
	GetJointPtr(jointID)->SetFinalTransform(updatedTransform);

	for (int const& childID : GetJointPtr(jointID)->GetChildJointIDs())
	{
		TransformJointAndChildren(childID, transform);
	}
}

void Skeleton::SetJointUniforms(Shader& skinnedShader)
{
	// Render Pipeline Entry point, setting shader's Joint Matrices
	for (auto const& IDNameJoint : m_Joints)
	{
		std::string jntUniformName = "aJoints[" + std::to_string(IDNameJoint.first.first) + "]";
		glm::mat4 OffsetTransform = IDNameJoint.second.GetFinalTransform()* IDNameJoint.second.GetModelInverseBindTransform();
		skinnedShader.setUniform(jntUniformName, OffsetTransform);
	}
}

void Skeleton::CalculateJointLocalBindOffsetTransforms()
{
	// Init Skeleton, calculating local joint offset to parent
	for (auto const& IDNameJoint : m_Joints)
	{
		if (IDNameJoint.second.GetParentJointID() != -1) // root joint has no parent
		{
			glm::mat4 parentModelBindTransform = GetJointPtr(IDNameJoint.second.GetParentJointID())->GetModelBindTransform();
			glm::mat4 currentInverseModelBindTransform = GetJointPtr(IDNameJoint.first.first)->GetModelInverseBindTransform();
			glm::mat4 localModelBindTransform = glm::inverse(parentModelBindTransform * currentInverseModelBindTransform);
			GetJointPtr(IDNameJoint.first.first)->SetLocalBindTransform(localModelBindTransform);
		}
	}
}


Skeleton::Skeleton()
{
}


Skeleton::~Skeleton()
{
}
