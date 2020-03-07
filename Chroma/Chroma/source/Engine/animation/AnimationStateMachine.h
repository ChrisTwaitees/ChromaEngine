#ifndef CHROMA_ANIMATIONSTATEMACHINE_H
#define CHROMA_ANIMATIONSTATEMACHINE_H

#include <statemachine/IStateMachine.h>

class AnimationStateMachine : public IStateMachine
{
public:
	AnimationStateMachine() {};
	~AnimationStateMachine() {};
	
	void TranstionTo(State const& newState);

private:
	bool test;
};

#endif