#ifndef CHROMA_ANIMATIONSTATEMACHINE_H
#define CHROMA_ANIMATIONSTATEMACHINE_H

#include <statemachine/IStateMachine.h>
#include <uid/UID.h>

class Animator;
struct Take;

class AnimationStateMachine : public IStateMachine
{
public:
	void Update() override;
	void Destroy() override;
	void ProcessConditions() override {};
	
	virtual void ProcessAnimator() {};
	virtual void TranstionTo(State const& newState);

	void SetAnimationComponentUID(UID const& animcompUID) { m_AnimationComponentUID = animcompUID; };

	Animator& GetAnimator();
	Take& GetTake(std::string const& takeName);

	AnimationStateMachine() {};
	~AnimationStateMachine() {};
private:

	UID m_AnimationComponentUID;
	
};

#endif