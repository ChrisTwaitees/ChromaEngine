#include "Animator.h"



void Animator::TriggerTimer(float const& duration, float& counter)
{
//	IWorker::AddJob(this->TriggerTimer);
}

void Animator::DoAnimation(Time& time)
{

	std::cout << "Processing Active Animations" << std::endl;
	for (Animation const& animation : m_Animations)
	{
		for (AnimTake const& animTake : animation.GetAnimTakes())
		{
			std::cout << "Animation Take Name : " << animTake.m_Name << std::endl;

		}
	}
	std::cout << "Animator Reading Delta time : " << time.GetDeltaTime() << " Game time : " << time.GetGameTime() << std::endl;

	if (m_Skeleton)
	{
		glm::vec3 rotationAxis{ 0.0, 1.0, 0.0 };
		float rotationAmount = glm::radians(glm::sin(time.GetGameTime() * 0.5) * 360);
		glm::mat4 rotationTransform =m_Skeleton->GetRootTransform() * glm::rotate(glm::mat4(1), rotationAmount, rotationAxis);


		m_Skeleton->TransformJointAndChildren( m_Skeleton->GetJointID("mixamorig:RightArm"), rotationTransform);
		//m_Skeleton->SetRotation(glm::angleAxis(rotationAmount, rotationAxis));
	}


}

Animator::Animator()
{
}


Animator::~Animator()
{
}
