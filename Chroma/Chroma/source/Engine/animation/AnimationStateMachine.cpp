#include "AnimationStateMachine.h"
#include <animation/Animator.h>

void AnimationStateMachine::Update()
{
	CHROMA_INFO("Animation State Machine Updating");
	for (std::pair<std::string, Take> const& take : m_Animator->GetTakes())
	{
		//CHROMA_INFO("Take : {0}", take.first);
	}
}

void AnimationStateMachine::TranstionTo(State const& newState)
{
	CHROMA_INFO("ANIM STATE MACHINE :: Transitioning to State : ", newState.m_Name);
	if (newState.m_TransitionFunc != nullptr)
	{
		CHROMA_INFO("ANIM STATE MACHINE :: Transition func called.");

	}
} 
