#include "AnimationStateMachine.h"
#include <animation/Animator.h>
#include <component/AnimationComponent.h>
#include <scene/Scene.h>
#include <input/Input.h>

void AnimationStateMachine::Update()
{
	CHROMA_INFO("Animation State Machine Updating");
	CHROMA_INFO("Current State : {}", m_CurrentState.m_Name);
	

}

void AnimationStateMachine::Destroy()
{
}

void AnimationStateMachine::TranstionTo(State const& newState)
{
	CHROMA_INFO("ANIM STATE MACHINE :: Transitioning from State {0} to {1} : ",m_CurrentState.m_Name, newState.m_Name);

	m_CurrentState.m_TransitionFromFunc();

	m_CurrentState = newState;

	m_CurrentState.m_TransitionToFunc();


}
Animator& AnimationStateMachine::GetAnimator()
{
	return static_cast<AnimationComponent*>(Chroma::Scene::GetComponent(m_AnimationComponentUID))->GetAnimator();
}

