#include "AnimationComponent.h"



void AnimationComponent::ProcessAnimators()
{
	for (Animator& animator : m_Animators)
	{
		std::cout << "Processing Animator : " << std::endl; 
		animator.DoAnimation();
	}
}

AnimationComponent::AnimationComponent()
{
}


AnimationComponent::~AnimationComponent()
{
}
