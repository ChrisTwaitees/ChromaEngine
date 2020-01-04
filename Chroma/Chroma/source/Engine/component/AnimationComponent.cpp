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
	{ CHROMA_INFO("Updating Animation Component : {0}", GetUID()); }
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
