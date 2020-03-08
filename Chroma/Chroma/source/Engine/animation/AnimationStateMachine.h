#ifndef CHROMA_ANIMATIONSTATEMACHINE_H
#define CHROMA_ANIMATIONSTATEMACHINE_H

#include <statemachine/IStateMachine.h>
#include <uid/UID.h>
class Animator;

class AnimationStateMachine : public IStateMachine
{
public:
	void Update();
	void Destroy();
	void TranstionTo(State const& newState);
	void SetAnimationComponentUID(UID const& animcompUID) { m_AnimationComponentUID = animcompUID; };

	Animator& GetAnimator();

	AnimationStateMachine() {};
	~AnimationStateMachine() {};
private:

	UID m_AnimationComponentUID;
};

#endif