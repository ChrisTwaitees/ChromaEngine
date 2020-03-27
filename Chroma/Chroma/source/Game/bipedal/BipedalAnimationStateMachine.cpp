#include "BipedalAnimationStateMachine.h"

#include <component/CharacterControllerComponent.h>
#include <animation/Animator.h>
#include <input/Input.h>
#include <time/Time.h>

typedef std::pair<std::string, Take> TakeData;


void BipedalAnimationStateMachine::Update()
{
	CHROMA_INFO("Bipedal State Machine Updating");
	CHROMA_INFO("Current State : {0}", m_CurrentState.m_Name);

	ProcessConditions();

	ProcessAnimator();

}

void BipedalAnimationStateMachine::Destroy()
{
	CHROMA_INFO("Bipedal State Machine Destroyed");
}

void BipedalAnimationStateMachine::ProcessConditions()
{
	CHROMA_INFO("Processing State : {0} TransitionConditions", m_CurrentState.m_Name);
	for (std::pair<AnimState, AnimStateTransitionCondition> transition : *m_CurrentState.m_Transitions)
	{
		//CHROMA_INFO("Condition for state : {}", transition.first.m_Name);
		if (transition.second.m_Condition(GetCharacterController()))
		{
			TranstionTo(transition.first);
			return;
		}
	}
}

void BipedalAnimationStateMachine::ProcessAnimator()
{

	// check whether currently transitioning
	if (m_IsTransitioning)
	{
		CHROMA_INFO("Transition Timer : {}", m_TransitionTimer);
		GetAnimator().LerpTakes(std::make_pair(0.5, Take()), std::make_pair(0.5, Take()));

		if (m_TransitionTimer <= 0.0f)
		{
			m_IsTransitioning = false;
		}
	}
	else // continue in current state
	{
		if (m_CurrentState.m_IsLooping)
		{
			GetAnimator().PlayTake(m_CurrentState.m_Name, Chroma::Time::GetLoopingTimeNormalized(GetTake(m_CurrentState.m_Name).m_Duration));
		}
		else
		{
			GetAnimator().PlayTake(m_CurrentState.m_Name, Chroma::Time::GetLoopingTimeNormalized(GetTake(m_CurrentState.m_Name).m_Duration));
		}
	}
}

void BipedalAnimationStateMachine::TranstionTo(AnimState const& newState)
{
	CHROMA_INFO("ANIM STATE MACHINE :: Transitioning from State {0} to {1} : ", m_CurrentState.m_Name, newState.m_Name);

	// call exit func
	if (m_CurrentState.m_Exit != nullptr)
	{
		m_CurrentState.m_Exit();
	}

	// call enter func
	if (m_CurrentState.m_Enter != nullptr)
	{
		m_CurrentState.m_Enter();
	}

	// set current state
	m_CurrentState = newState;

	// trigger transition timer
	if (m_CurrentState.m_TransitionTime != 0.0f)
	{
		m_TransitionTimer = m_CurrentState.m_TransitionTime;
		Chroma::Time::StartNormalizedTimer(m_TransitionTimer);
		m_IsTransitioning = true;
	}
}


bool WalkTransitionCondition(CharacterControllerComponent* characterController)
{
	if (glm::length(characterController->GetVelocity()) > 0.1f)
	{
		CHROMA_INFO("Walk condition met!");
		return true;
	}
	else
		return false;
}


bool IdleTransitionCondition(CharacterControllerComponent* characterController)
{
	if (glm::length(characterController->GetVelocity()) < 0.1f)
	{
		CHROMA_INFO("Idle condition met!");
		return true;
	}
	else
		return false;
}

bool JumpTransitionCondition(CharacterControllerComponent* characterController)
{
	if (Chroma::Input::IsPressed(Chroma::Input::SPACEBAR) || Chroma::Input::IsPressed(Chroma::Input::CROSS))
	{
		CHROMA_INFO("Jump condition met!");
		return true;
	}
	else
		return false;
}

void BipedalAnimationStateMachine::Init()
{
	// Idle
	AnimState m_IdleState("Idle");
	m_IdleState.m_IsLooping = true;
	m_States.push_back(m_IdleState);

	// Walk
	AnimState m_WalkState("Walk");
	m_WalkState.m_IsLooping = true;
	m_States.push_back(m_WalkState);
	
	// Jump
	AnimState m_JumpState("Jump");
	m_JumpState.m_IsLooping = true;
	m_States.push_back(m_JumpState);


	// transitions
	// idle
	m_IdleState.m_Transitions->push_back(std::make_pair(m_WalkState , AnimStateTransitionCondition(&WalkTransitionCondition)));
	m_IdleState.m_Transitions->push_back(std::make_pair(m_JumpState, AnimStateTransitionCondition(&JumpTransitionCondition)));
	// walk
	m_WalkState.m_Transitions->push_back(std::make_pair(m_JumpState, AnimStateTransitionCondition(&JumpTransitionCondition)));
	m_WalkState.m_Transitions->push_back(std::make_pair(m_IdleState, AnimStateTransitionCondition(&IdleTransitionCondition)));
	// jump					 
	m_JumpState.m_Transitions->push_back(std::make_pair(m_IdleState, AnimStateTransitionCondition(&IdleTransitionCondition)));
	m_JumpState.m_Transitions->push_back(std::make_pair(m_WalkState, AnimStateTransitionCondition(&WalkTransitionCondition)));

	// set current state
	m_CurrentState = m_IdleState;

}