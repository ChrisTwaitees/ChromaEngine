#include "AnimationComponent.h"


void AnimationComponent::ProcessAnimators(Time& time)
{
	for (Animator& animator : m_Animators)
	{
		animator.DoAnimation(time);
	}
}

AnimationComponent::AnimationComponent()
{
}


AnimationComponent::~AnimationComponent()
{
}
