#include "AnimationComponent.h"
#include <entity/IEntity.h>

void AnimationComponent::UpdateDebug(std::string const& debugAnimClipName, float const& debugTime)
{
	m_Animator.DebugAnimationTake(debugAnimClipName, debugTime);
}


void AnimationComponent::Init()
{
	// Set Type
	m_Type = Chroma::Type::Component::kAnimationComponent;

	CMPNT_INITIALIZED;
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
	// Destroy animators
	m_Animator.Destroy();

	// Destroy State Machine
	if(m_AnimationStateMachine)
		m_AnimationStateMachine->Destroy();

	CMPNT_DESTROYED
}

void AnimationComponent::Serialize(ISerializer*& serializer)
{
	CMPNT_SERIALIZE_BEGIN


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
