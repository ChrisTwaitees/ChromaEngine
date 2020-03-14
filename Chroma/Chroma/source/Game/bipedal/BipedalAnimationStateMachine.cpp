#include "BipedalAnimationStateMachine.h"
#include <animation/Animator.h>
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
	CHROMA_INFO("Bipedal State Machine Destoyed");
}

void BipedalAnimationStateMachine::ProcessConditions()
{
	CHROMA_INFO("Processing State : {0} TransitionConditions", m_CurrentState.m_Name);
	for (std::pair<State, StateTransitionCondition> transition : m_CurrentState.m_Transitions)
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
	GetAnimator().PlayTake(m_CurrentState.m_Name, Chroma::Time::GetLoopingTimeNormalized(GetTake(m_CurrentState.m_Name).m_Duration));
}

void BipedalAnimationStateMachine::Init()
{
	// Idle
	State m_IdleState("Idle");
	//m_IdleState.m_Enter = Destroy;

	m_States.push_back(m_IdleState);


	m_CurrentState = m_IdleState;

	// Idle
	State m_WalkState("Walk");

	m_States.push_back(m_WalkState);
}