#include "AnimationComponent.h"


void AnimationComponent::UpdateDebug(std::string const& debugAnimClipName, float const& debugTime)
{
	for (Animator& animator : m_Animators)
	{
		animator.DebugAnimationTake(debugAnimClipName, debugTime);
	}
}


void AnimationComponent::Init()
{
	CHROMA_TRACE("AnimationComponent : {0} Initialized.", m_UID.data);
}

void AnimationComponent::Update()
{
	for (Animator& animator : m_Animators)
	{
		animator.DoAnimation();
	}
}

void AnimationComponent::Destroy()
{
	CHROMA_TRACE("AnimationComponent : {0} Destroyed.", m_UID.data);
}

AnimationComponent::AnimationComponent()
{
}


AnimationComponent::~AnimationComponent()
{
}
