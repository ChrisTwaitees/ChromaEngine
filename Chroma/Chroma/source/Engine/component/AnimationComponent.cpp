#include "AnimationComponent.h"


void AnimationComponent::ProcessAnimators(Time& time)
{
	for (Animator& animator : m_Animators)
	{
		animator.DoAnimation(time);
	}
}

void AnimationComponent::ProcessAnimatorsDebug(std::string const& debugAnimClipName, float const& debugTime)
{
	for (Animator& animator : m_Animators)
	{
		animator.DebugAnimationTake(debugAnimClipName, debugTime);
	}
}

AnimationComponent::AnimationComponent()
{
}


AnimationComponent::~AnimationComponent()
{
}
