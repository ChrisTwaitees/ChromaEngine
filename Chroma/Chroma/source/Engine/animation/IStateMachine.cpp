#include "IStateMachine.h"

void IStateMachine::Init()
{
	CHROMA_INFO("State Machine Initialized.");
}

IStateMachine::IStateMachine()
{
	Init();
}

IStateMachine::~IStateMachine()
{
}
