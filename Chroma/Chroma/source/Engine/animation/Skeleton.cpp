#include "Skeleton.h"




void Skeleton::AddJoint(Joint& newJoint)
{
	m_Joints[make_pair(newJoint.GetID(), newJoint.GetName())] = newJoint;
}

void Skeleton::SetGlobalTransform(glm::mat4 const& newGlobalTransform)
{
	m_GlobalTransform = newGlobalTransform;
	m_GlobalTransformInverse = glm::inverse(newGlobalTransform);
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
	for (auto const& IDNameJoint : m_Joints)
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
	for (auto const& IDNameJoint : m_Joints)
	{
		if (IDNameJoint.first.second == jointName)
		{
			return IDNameJoint.second;
		}
	}
	std::cout << "::SKELETON ERROR::" << std::endl;
	std::cout << "JOINT NAME : " << jointName << " COULD NOT BE FOUND. " << std::endl;
	
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

void Skeleton::CalculateJointBindTransforms()
{
	ProcessChildModelBindTransforms(&GetRootJoint(), m_GlobalTransform);
}

void Skeleton::DebugDraw(DebugBuffer* debugBuffer)
{
	DebugWalkChildJoints(&GetRootJoint(), debugBuffer);
}

void Skeleton::DebugWalkChildJoints(Joint* currentJoint, DebugBuffer* const &debugBuffer)
{
	glm::vec3 startPos = GLMGetTranslation(currentJoint->GetLocalBindTransform());

	for (Joint& child : currentJoint->GetChildJoints())
	{
		glm::vec3 endPos = GLMGetTranslation(child.GetLocalBindTransform());
		debugBuffer->DrawLine(startPos, endPos, glm::vec3(1.0));
		DebugWalkChildJoints(&GetJoint(child.GetID()), debugBuffer);
	}
}

void Skeleton::ProcessChildModelBindTransforms(Joint* currentJoint, glm::mat4 const& parentTransform)
{
	currentJoint->SetModelBindTransform(parentTransform * currentJoint->GetLocalBindTransform());
	currentJoint->SetModelInverseBindTransform(glm::inverse(currentJoint->GetModelBindTransform()));

	for (Joint& child : currentJoint->GetChildJoints())
	{
		ProcessChildModelBindTransforms(&GetJoint(child.GetID()), currentJoint->GetModelBindTransform());
	}
}


Skeleton::Skeleton()
{
}


Skeleton::~Skeleton()
{
}
