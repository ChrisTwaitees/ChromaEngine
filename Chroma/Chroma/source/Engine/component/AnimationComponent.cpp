#include "AnimationComponent.h"


void AnimationComponent::UpdateDebug(std::string const& debugAnimClipName, float const& debugTime)
{
	for (Animator& animator : m_Animators)
	{
		animator.DebugAnimationTake(debugAnimClipName, debugTime);
	}
}


void AnimationComponent::Update()
{
	for (Animator& animator : m_Animators)
	{
		animator.DoAnimation();
	}
}

AnimationComponent::AnimationComponent()
{
}


AnimationComponent::~AnimationComponent()
{
}
