#include "AnimationStateMachine.h"

#include <component/CharacterControllerComponent.h>
#include <component/AnimationComponent.h>
#include <animation/Animator.h>
#include <scene/Scene.h>
#include <input/Input.h>

namespace Chroma
{

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
		if (Scene::GetComponent(m_AnimationComponentUID)->GetParentEntity()->GetCharacterControllerComponentUIDs().size() > 0)
		{
			UID CharacterControllerUID = Scene::GetComponent(m_AnimationComponentUID)->GetParentEntity()->GetCharacterControllerComponentUIDs()[0];
			return static_cast<CharacterControllerComponent*>(Scene::GetComponent(CharacterControllerUID));
		}
		else
			return nullptr;

	}

	Animator& AnimationStateMachine::GetAnimator()
	{
		return static_cast<AnimationComponent*>(Scene::GetComponent(m_AnimationComponentUID))->GetAnimator();
	}

}
