#include "IStateMachine.h"

void IStateMachine::Init()
{
	CHROMA_INFO("STATE MACHINE :: Initialized");
}

void IStateMachine::AddState(State const& newState)
{
	CHROMA_INFO("STATE MACHINE :: adding State {}", newState.m_Name);
	m_States.push_back(newState);
}

IStateMachine::IStateMachine()
{
	Init();
}

IStateMachine::~IStateMachine()
{
	CHROMA_WARN("IStateMachine Deleted.");
}
