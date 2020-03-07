#include "AnimationStateMachine.h"

void AnimationStateMachine::TranstionTo(State const& newState)
{
	CHROMA_INFO("ANIM STATE MACHINE :: Transitioning to State : ", newState.m_Name);
	if (newState.m_TransitionFunc != nullptr)
	{
		CHROMA_INFO("ANIM STATE MACHINE :: Transition func called.");

	}
} 
