#include "Skeleton.h"
#include <entity/IChromaEntity.h>



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
	DebugWalkChildJoints(GetRootJoint(), debugBuffer);
}

void Skeleton::DebugWalkChildJoints(Joint const& currentJoint, DebugBuffer* const &debugBuffer)
{
	glm::vec3 startPos = GLMGetTranslation(GetJoint(currentJoint.GetID()).GetModelBindTransform());

	for (Joint const& child : currentJoint.GetChildJoints())
	{
		glm::vec3 endPos = GLMGetTranslation(GetJoint(child.GetID()).GetModelBindTransform());
		debugBuffer->DrawOverlayLine(startPos, endPos, glm::vec3(1.0));
		DebugWalkChildJoints(GetJoint(child.GetID()), debugBuffer);
	}
}


void Skeleton::UpdateSkeletonRootTransform()
{
	glm::mat4 rootTransform = glm::translate(m_IdentityMatrix, m_Translation);
	rootTransform = glm::toMat4(m_Rotation) * rootTransform;
	rootTransform = glm::scale(rootTransform, glm::vec3(m_Scale));
	
	ProcessChildModelBindTransforms(m_RootJointID, rootTransform);
	//for (auto const& IDNameJoint : m_Joints)
	//{
	//	glm::mat4 newLocalBindTransform = translateMat * IDNameJoint.second.GetLocalBindTransform();
	//	GetJointPtr(IDNameJoint.first.first)->SetLocalBindTransform(newLocalBindTransform);
	//}
}


// Joint Hierarchy on Innit
void Skeleton::CalculateJointBindTransforms()
{
	ProcessChildModelBindTransforms(m_RootJointID, m_WorldTransform);
}

void Skeleton::ProcessChildModelBindTransforms(int const& jointID, glm::mat4 const& parentTransform)
{
	glm::mat4 modelBindTransform{ parentTransform * GetJointPtr(jointID)->GetLocalBindTransform() };
	GetJointPtr(jointID)->SetModelBindTransform(modelBindTransform);
	GetJointPtr(jointID)->SetModelInverseBindTransform(glm::inverse(modelBindTransform));
	for (Joint const& child : GetJointPtr(jointID)->GetChildJoints())
	{
		ProcessChildModelBindTransforms(child.GetID(), modelBindTransform);
	}
}


Skeleton::Skeleton()
{
}


Skeleton::~Skeleton()
{
}
