#include "Animator.h"



void Animator::TriggerTimer(float const& duration, float& counter)
{
//	IWorker::AddWork(this->TriggerTimer);
}

void Animator::DoAnimation(ChromaTime& time)
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
}

Animator::Animator()
{
}


Animator::~Animator()
{
}
