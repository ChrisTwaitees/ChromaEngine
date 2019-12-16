#include "Skeleton.h"


void Skeleton::AddJoint(Joint const& newJoint)
{
	m_NamedJoints[newJoint.GetName()] = newJoint;
	m_IndexedJoints[newJoint.GetID()] = newJoint;
	if (newJoint.GetID() == 0)
		m_RootJoint = newJoint;
}

void Skeleton::SetGlobalTransform(glm::mat4 const& newGlobalTransform)
{
	m_GlobalTransform = newGlobalTransform;
	m_GlobalTransformInverse = glm::inverse(newGlobalTransform);

}

Joint Skeleton::GetJoint(int& index)
{
	std::map<int, Joint>::iterator it;
	m_IndexedJoints[index];
	it = m_IndexedJoints.find(index);
	if (it != m_IndexedJoints.end())
		return it->second;
	else
	{
		std::cout << "::SKELETON ERROR::" << std::endl;
		std::cout << "JOINT ID : " << index << " COULD NOT BE FOUND IN SKELETON. " << std::endl;
	}
}

Joint Skeleton::GetJoint(std::string const&  jointName) 
{
	std::map<std::string, Joint>::iterator it;
	it = m_NamedJoints.find(jointName);
	if (it != m_NamedJoints.end())
		return it->second;
	else
	{
		std::cout << "::SKELETON ERROR::" << std::endl;
		std::cout << "JOINT NAME : " << jointName << " COULD NOT BE FOUND IN SKELETON. " << std::endl;
	}
}

void Skeleton::CalculateJointBindTransforms()
{
	m_RootJoint.CalculateInverseModelBindTransforms(m_GlobalTransform);
}

Skeleton::Skeleton()
{
}


Skeleton::~Skeleton()
{
}
