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

	CHROMA_INFO("Velocity : {0}, {1}, {2}", GetCharacterController()->GetVelocity()[0], GetCharacterController()->GetVelocity()[1], GetCharacterController()->GetVelocity()[2]);

}

void BipedalAnimationStateMachine::Destroy()
{
	CHROMA_INFO("Bipedal State Machine Destroyed");
}

void BipedalAnimationStateMachine::ProcessConditions()
{
	CHROMA_INFO("Processing State : {0} TransitionConditions", m_CurrentState.m_Name);
	for (std::pair<AnimState, StateTransitionCondition> transition : m_CurrentState.m_Transitions)
	{
		if (transition.second.m_Condition())
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
		// TODO : Animator func to interpolate two takes according to normalized timer
		// 
	}
	else
	{
		GetAnimator().PlayTake(m_CurrentState.m_Name, Chroma::Time::GetLoopingTimeNormalized(GetTake(m_CurrentState.m_Name).m_Duration));
	}
}

void BipedalAnimationStateMachine::TranstionTo(AnimState const& newState)
{
	CHROMA_INFO("ANIM STATE MACHINE :: Transitioning from State {0} to {1} : ", m_CurrentState.m_Name, newState.m_Name);

	if (m_CurrentState.m_Exit != nullptr)
	{
		m_CurrentState.m_Exit();
	}

	m_CurrentState = newState;

	if (m_CurrentState.m_Enter != nullptr)
	{
		m_CurrentState.m_Enter();
	}

}


bool WalkTransitionCondition()
{
	if (Chroma::Input::GetAxis("Vertical") > 0.1f || Chroma::Input::GetAxis("Vertical") < -0.1f)
	{
		CHROMA_INFO("Walk condition met!");
		return true;
	}
	else
		return false;
}


bool IdleTransitionCondition()
{
	if (Chroma::Input::GetAxis("Vertical") < 0.1f || Chroma::Input::GetAxis("Vertical") > -0.1f)
	{
		CHROMA_INFO("Idle condition met!");
		return true;
	}
	else
		return false;
}

bool JumpTransitionCondition()
{
	if (Chroma::Input::IsPressed(Chroma::Input::SPACEBAR))
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
	m_States.push_back(m_IdleState);

	// Idle
	AnimState m_WalkState("Walk");
	m_States.push_back(m_WalkState);
	
	// Jump
	AnimState m_JumpState("Jump");
	m_States.push_back(m_JumpState);

	// transitions
	// idle
	m_IdleState.m_Transitions.push_back(std::make_pair(m_WalkState , StateTransitionCondition(&WalkTransitionCondition)));
	// walk
	m_WalkState.m_Transitions.push_back(std::make_pair(m_IdleState, StateTransitionCondition(&IdleTransitionCondition)));
	m_WalkState.m_Transitions.push_back(std::make_pair(m_JumpState, StateTransitionCondition(&JumpTransitionCondition)));
	// jump
	m_JumpState.m_Transitions.push_back(std::make_pair(m_IdleState, StateTransitionCondition(&IdleTransitionCondition)));
	m_JumpState.m_Transitions.push_back(std::make_pair(m_WalkState, StateTransitionCondition(&WalkTransitionCondition)));

	// set current state
	m_CurrentState = m_IdleState;
}