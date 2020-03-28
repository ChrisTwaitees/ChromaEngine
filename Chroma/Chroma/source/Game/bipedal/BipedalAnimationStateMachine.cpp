#include "BipedalAnimationStateMachine.h"

#include <component/CharacterControllerComponent.h>
#include <animation/Animator.h>
#include <input/Input.h>
#include <time/Time.h>

typedef std::pair<std::string, Take> TakeData;


void BipedalAnimationStateMachine::Update()
{
	CHROMA_INFO_UNDERLINE;

	CHROMA_INFO("Bipedal State Machine Updating");
	CHROMA_INFO("Current State : {0}, time : {1}", m_CurrentState.m_Name, m_CurrentState.m_CurrentTime);
	CHROMA_INFO("Previous State : {0}, time : {1}", m_PreviousState.m_Name, m_PreviousState.m_CurrentTime);

	ProcessAnimStates(); 

	ProcessConditions();

	ProcessAnimator();

	CHROMA_INFO_UNDERLINE;
}

void BipedalAnimationStateMachine::Destroy()
{
	CHROMA_INFO("Bipedal State Machine Destroyed");
}

void BipedalAnimationStateMachine::ProcessConditions()
{
	CHROMA_INFO("Processing StateTransition Conditions : {0}", m_CurrentState.m_Name);
	for (std::pair<AnimState, AnimStateTransitionCondition> const& transition : *m_CurrentState.m_Transitions)
	{
		//CHROMA_INFO("Condition for state : {}", transition.first.m_Name);
		if (transition.second.m_Condition(GetCharacterController()))
		{
			TranstionTo(transition.first);
			return;
		}
	}
}

void BipedalAnimationStateMachine::ProcessAnimStates()
{
	// Updates current and previous states Current Time
	CHROMA_INFO("Processing Anim States");


	// if in transition continue updating previous state
	if (m_IsTransitioning)
	{
		if (m_PreviousState.m_IsLooping)
		{
			m_PreviousState.m_CurrentTime = Chroma::Time::GetLoopingTimeNormalized(GetTake(m_PreviousState.m_Name).m_Duration);
		}
	}

	// update time in current state
	if (m_CurrentState.m_IsLooping)
	{
		m_CurrentState.m_CurrentTime = Chroma::Time::GetLoopingTimeNormalized(GetTake(m_CurrentState.m_Name).m_Duration);
	}

}

void BipedalAnimationStateMachine::ProcessAnimator()
{
	// check whether currently transitioning
	m_IsTransitioning = m_TransitionTimer != 1.0f;

	if (m_IsTransitioning)
	{
		CHROMA_INFO("Transition Timer : {}", m_TransitionTimer);
		// Lerp Takes
		GetAnimator().LerpTakes(std::make_pair(m_PreviousState.m_Name, m_PreviousState.m_CurrentTime),
			std::make_pair(m_CurrentState.m_Name, m_CurrentState.m_CurrentTime), m_TransitionTimer);
	}
	else // continue in current state
	{
		GetAnimator().PlayTake(m_CurrentState.m_Name, m_CurrentState.m_CurrentTime);
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

	// set previous state
	m_PreviousState = m_CurrentState;

	// set current state
	m_CurrentState = newState;

	// trigger transition timer
	if (m_CurrentState.m_TransitionTime != 0.0f)
	{
		m_TransitionTimer = m_CurrentState.m_TransitionTime;
		Chroma::Time::StartNormalizedTimer01(m_TransitionTimer);
		m_IsTransitioning = true;
	}

	// trigger once off timer if not looping
	if (!m_CurrentState.m_IsLooping)
	{
		m_CurrentState.m_CurrentTime = GetTake(m_CurrentState.m_Name).m_Duration;
		Chroma::Time::StartNormalizedTimer01(m_CurrentState.m_CurrentTime);
	}

	// call enter func
	if (m_CurrentState.m_Enter != nullptr)
	{
		m_CurrentState.m_Enter();
	}

}


bool WalkTransitionCondition(CharacterControllerComponent* characterController)
{
	if (characterController->GetIsOnGround())
	{
		if (glm::length(characterController->GetVelocity()) > 0.1f && glm::length(characterController->GetVelocity()) < 0.3f)
		{
			CHROMA_INFO("Walk condition met!");
			return true;
		}
		else
			return false;
	}
	else
		return false;
}


bool IdleTransitionCondition(CharacterControllerComponent* characterController)
{
	if (characterController->GetIsOnGround())
	{
		if (glm::length(characterController->GetVelocity()) < 0.1f)
		{
			CHROMA_INFO("Idle condition met!");
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool SprintTransitionCondition(CharacterControllerComponent* characterController)
{
	if (characterController->GetIsOnGround())
	{
		if (glm::length(characterController->GetVelocity()) > 0.3f)
		{
			CHROMA_INFO("Sprint condition met!");
			return true;
		}
		else
			return false;
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

	// Sprint
	AnimState m_RunState("Run");
	m_RunState.m_IsLooping = true;
	m_States.push_back(m_RunState);
	
	// Jump
	AnimState m_JumpState("Jump");
	m_JumpState.m_IsLooping = false;
	m_JumpState.m_TransitionTime = 0.1f;
	m_States.push_back(m_JumpState);


	// transitions
	// idle
	m_IdleState.m_Transitions->push_back(std::make_pair(m_WalkState , AnimStateTransitionCondition(&WalkTransitionCondition)));
	m_IdleState.m_Transitions->push_back(std::make_pair(m_JumpState, AnimStateTransitionCondition(&JumpTransitionCondition)));
	// walk
	m_WalkState.m_Transitions->push_back(std::make_pair(m_JumpState, AnimStateTransitionCondition(&JumpTransitionCondition)));
	m_WalkState.m_Transitions->push_back(std::make_pair(m_IdleState, AnimStateTransitionCondition(&IdleTransitionCondition)));
	m_WalkState.m_Transitions->push_back(std::make_pair(m_RunState, AnimStateTransitionCondition(&SprintTransitionCondition)));
	// sprint
	m_RunState.m_Transitions->push_back(std::make_pair(m_JumpState, AnimStateTransitionCondition(&JumpTransitionCondition)));
	m_RunState.m_Transitions->push_back(std::make_pair(m_IdleState, AnimStateTransitionCondition(&IdleTransitionCondition)));
	m_RunState.m_Transitions->push_back(std::make_pair(m_WalkState, AnimStateTransitionCondition(&WalkTransitionCondition)));
	// jump					 
	m_JumpState.m_Transitions->push_back(std::make_pair(m_IdleState, AnimStateTransitionCondition(&IdleTransitionCondition)));
	m_JumpState.m_Transitions->push_back(std::make_pair(m_WalkState, AnimStateTransitionCondition(&WalkTransitionCondition)));

	// set current state
	m_CurrentState = m_IdleState;

}