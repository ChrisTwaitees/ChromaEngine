#include "Animator.h"



void Animator::LoadAnimations(std::string const& sourcePath)
{
	m_Takes = AnimationLoader::LoadAnimations(sourcePath);
}

void Animator::TriggerTimer(float const& duration, float& counter)
{
//	IWorker::AddJob(this->TriggerTimer);
}

void Animator::DoAnimation(Time& time)
{
	if (m_Skeleton == nullptr)
	{
		std::cout << "::ANIMATOR ERROR:: No Skeleton Found, cannot perform animation" << std::endl;
		return;
	}

	// After selecting which animation to play process animation
	//m_Takes[0].GetAnimTakes()[0].m_KeyFrames[0];

	/*std::cout << "Processing Active Animations" << std::endl;
	for (Animation const& animation : m_Takes)
	{
		for (AnimTake const& animTake : animation.GetAnimTakes())
		{
			std::cout << "Animation Take Name : " << animTake.m_Name << std::endl;

		}
	}*/
}

Animator::Animator()
{
}


Animator::~Animator()
{
}
