#include "AnimationStateMachine.h"

#include <component/CharacterControllerComponent.h>
#include <component/AnimationComponent.h>
#include <animation/Animator.h>
#include <scene/Scene.h>
#include <input/Input.h>

void AnimationStateMachine::Update()
{
	CHROMA_INFO("Animation State Machine Updating");
	CHROMA_INFO("Current State : {}", m_CurrentState.m_Name);
}

void AnimationStateMachine::Destroy()
{
	CHROMA_INFO("Animation State Machine Destroyed");
}

Take& AnimationStateMachine::GetTake(std::string const& takeName)
{
	return GetAnimator().GetTake(takeName);
}

void AnimationStateMachine::TranstionTo(AnimState const& newState)
{
	CHROMA_INFO("ANIM STATE MACHINE :: Transitioning from State {0} to {1} : ",m_CurrentState.m_Name, newState.m_Name);

	m_CurrentState.m_Exit();

	m_CurrentState = newState;

	m_CurrentState.m_Enter();
}

CharacterControllerComponent* AnimationStateMachine::GetCharacterController()
{
	UID CharacterControllerUID = Chroma::Scene::GetComponent(m_AnimationComponentUID)->GetParentEntity()->GetCharacterControllerComponentUIDs()[0];
	return static_cast<CharacterControllerComponent*>(Chroma::Scene::GetComponent(CharacterControllerUID));
	
	// TODO: insert return statement here
}

Animator& AnimationStateMachine::GetAnimator()
{
	return static_cast<AnimationComponent*>(Chroma::Scene::GetComponent(m_AnimationComponentUID))->GetAnimator();
}

