#include "AnimationComponent.h"
#include <entity/IEntity.h>

void AnimationComponent::UpdateDebug(std::string const& debugAnimClipName, float const& debugTime)
{
	m_Animator.DebugAnimationTake(debugAnimClipName, debugTime);
}


void AnimationComponent::Init()
{
	CHROMA_TRACE("AnimationComponent : {0} Initialized.", m_UID.data);
}

void AnimationComponent::Update()
{
	// state machinem_AnimationComponentUID
	if( m_AnimationStateMachine != nullptr)
		m_AnimationStateMachine->Update();

	// animator
	m_Animator.Update();
}

void AnimationComponent::Destroy()
{
	// remove animators
	m_Animator.Destroy();
	m_AnimationStateMachine->Destroy();

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
	m_Animator.SetAnimationComponentUID(m_UID);
}

void AnimationComponent::SetAnimationStateMachine(AnimationStateMachine*& newAnimationStateMachine)
{
	// Set animation state machine
	m_AnimationStateMachine = newAnimationStateMachine;
	// Set UID
	m_AnimationStateMachine->SetAnimationComponentUID(m_UID);
}


AnimationComponent::AnimationComponent()
{
}


AnimationComponent::~AnimationComponent()
{
	delete m_AnimationStateMachine;
}
