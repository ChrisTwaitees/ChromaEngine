#ifndef CHROMA_ANIMATIONSTATEMACHINE_H
#define CHROMA_ANIMATIONSTATEMACHINE_H

#include <statemachine/IStateMachine.h>
class Animator;

class AnimationStateMachine : public IStateMachine
{
public:
	void Update();
	void TranstionTo(State const& newState);
	AnimationStateMachine(Animator* newAnimator) : m_Animator(newAnimator) {};
	~AnimationStateMachine() {};
private:
	Animator* m_Animator;
};

#endif