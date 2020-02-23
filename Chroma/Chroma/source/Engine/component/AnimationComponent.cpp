#include "AnimationComponent.h"
#include <entity/IEntity.h>

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
	for (UID const& uid : GetParentEntity()->getCharacterControllerComponentUIDs())
	{
		SetCharacterControllerComponentUID(uid);
	}
}

void AnimationComponent::Update()
{
	for (Animator& animator : m_Animators)
	{
		animator.Update();
	}
}

void AnimationComponent::Destroy()
{
	// remove animators
	for (Animator& animator : m_Animators)
	{
		animator.Destroy();
	}
	m_Animators.clear();

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

void AnimationComponent::AddAnimator(Animator& newAnimator)
{
	// Set UID
	newAnimator.SetAnimationComponentUID(m_UID);
	// Add to existing animators
	m_Animators.push_back(newAnimator);

}

void AnimationComponent::SetCharacterControllerComponentUID(UID const& newCharacterControllerComponentUID)
{
	for (Animator& animator : m_Animators)
	{
		animator.SetCharacterControllerComponentUID(newCharacterControllerComponentUID);
	}
}

AnimationComponent::AnimationComponent()
{
}


AnimationComponent::~AnimationComponent()
{
}
