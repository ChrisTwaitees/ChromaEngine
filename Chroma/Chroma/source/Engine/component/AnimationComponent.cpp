#include "AnimationComponent.h"


void AnimationComponent::ProcessAnimators(ChromaTime& time)
{
	for (Animator& animator : m_Animators)
	{
		std::cout << "Processing Animator : " << std::endl;
		animator.DoAnimation(time);
	}
}

AnimationComponent::AnimationComponent()
{
}


AnimationComponent::~AnimationComponent()
{
}
