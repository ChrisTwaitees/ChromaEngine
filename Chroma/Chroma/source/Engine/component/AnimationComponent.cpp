#include "AnimationComponent.h"


void AnimationComponent::ProcessAnimators()
{
	for (Animator& animator : m_Animators)
	{
		animator.DoAnimation();
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
