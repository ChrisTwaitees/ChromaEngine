#include "AnimationStateMachine.h"
#include <animation/Animator.h>
#include <component/AnimationComponent.h>
#include <scene/Scene.h>
#include <input/Input.h>

void AnimationStateMachine::Update()
{
	CHROMA_INFO("Animation State Machine Updating");
	for (std::pair<std::string, Take> const& take : GetAnimator().GetTakes())
	{
		//CHROMA_INFO("Take : {0}", take.first);
	}
	CHROMA_INFO("Vertical : {}", Chroma::Input::GetAxis("Vertical"));
	CHROMA_INFO("Horizontal : {}", Chroma::Input::GetAxis("Horizontal"));
}

void AnimationStateMachine::Destroy()
{
}

void AnimationStateMachine::TranstionTo(State const& newState)
{
	CHROMA_INFO("ANIM STATE MACHINE :: Transitioning to State : ", newState.m_Name);
	if (newState.m_TransitionFunc != nullptr)
	{
		CHROMA_INFO("ANIM STATE MACHINE :: Transition func called.");

	}
}
Animator& AnimationStateMachine::GetAnimator()
{
	return static_cast<AnimationComponent*>(Chroma::Scene::GetComponent(m_AnimationComponentUID))->GetAnimator();
}

