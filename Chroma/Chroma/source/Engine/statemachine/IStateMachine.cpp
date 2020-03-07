#include "IStateMachine.h"

void IStateMachine::Init()
{
	CHROMA_INFO("STATE MACHINE :: Initialized");
}

void IStateMachine::Update()
{
	CHROMA_INFO("STATE MACHINE :: Updating");
	CHROMA_INFO("STATE MACHINE :: Current State : {0}", m_CurrentState.m_Name);
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
}
