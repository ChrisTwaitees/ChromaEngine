#include "AnimationComponent.h"
#include <entity/IEntity.h>

void AnimationComponent::UpdateDebug(std::string const& debugAnimClipName, float const& debugTime)
{
	m_Animator.DebugAnimationTake(debugAnimClipName, debugTime);
}


void AnimationComponent::Init()
{
	CHROMA_TRACE("AnimationComponent : {0} Initialized.", m_UID.data);
	for (UID const& uid : GetParentEntity()->getCharacterControllerComponentUIDs())
	{
		SetCharacterControllerComponentUID(uid);
	}
}

void AnimationComponent::Update()
{
	m_AnimationStateMachine.Update();
	m_Animator.Update();
}

void AnimationComponent::Destroy()
{
	// remove animators
	m_Animator.Destroy();
	m_AnimationStateMachine.Destroy();

	CHROMA_TRACE("AnimationComponent : {0} Destroyed.", m_UID.data);
}

void AnimationComponent::Serialize(ISerializer*& serializer)
{
	CHROMA_INFO("Serializing Animation Component : {0}", m_UID.data);
	serializer->StartObject("AnimationComponent", m_UID);

	serializer->AddProperty("Skeleton", "testSkeleton");
	serializer->AddProperty("Skeleton", "testSkeleton");
	serializer->AddProperty("Skeleton", "testSkeleton");
}

void AnimationComponent::SetAnimator(Animator& newAnimator)
{
	// Set Animator
	m_Animator = newAnimator;
	// Set UID
	newAnimator.SetAnimationComponentUID(m_UID);

}

void AnimationComponent::SetAnimationStateMachine(AnimationStateMachine& newAnimationStateMachine)
{
	// Set animation state machine
	m_AnimationStateMachine = newAnimationStateMachine;
	// Set UID
	m_AnimationStateMachine.SetAnimationComponentUID(m_UID);
}

void AnimationComponent::SetCharacterControllerComponentUID(UID const& newCharacterControllerComponentUID)
{
	m_Animator.SetCharacterControllerComponentUID(newCharacterControllerComponentUID);
}

AnimationComponent::AnimationComponent()
{
}


AnimationComponent::~AnimationComponent()
{
}
