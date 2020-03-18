#ifndef CHROMA_ANIMATIONSTATEMACHINE_H
#define CHROMA_ANIMATIONSTATEMACHINE_H

#include <statemachine/IStateMachine.h>
#include <uid/UID.h>

class Animator;
struct Take;

struct AnimState : public State
{
	bool m_IsLooping{false};
	float m_TransitionTime{ 0.0f };
	std::vector<std::pair<AnimState, StateTransitionCondition>> m_Transitions;
	AnimState() {};
	AnimState(std::string const& takeName) { m_Name = takeName; };
};


class AnimationStateMachine : public IStateMachine
{
public:
	void Update() override;
	void Destroy() override;
	void ProcessConditions() override {};
	
	virtual void ProcessAnimator() {};
	virtual void TranstionTo(AnimState const& newState);

	void SetAnimationComponentUID(UID const& animcompUID) { m_AnimationComponentUID = animcompUID; };

	Animator& GetAnimator();
	Take& GetTake(std::string const& takeName);

	AnimationStateMachine() {};
	~AnimationStateMachine() {};

protected:
	UID m_AnimationComponentUID;
	std::vector<AnimState> m_States;
	AnimState m_CurrentState;
	float m_TransitionTimer{ 0.0f };
	bool m_IsTransitioning{ false };
	
};

#endif