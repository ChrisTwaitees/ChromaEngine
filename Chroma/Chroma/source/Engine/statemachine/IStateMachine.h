#ifndef _CHROMA_STATE_MACHINE_H_
#define _CHROMA_STATE_MACHINE_H_

//common
#include <common/PrecompiledHeader.h>

struct StateTransitionCondition
{
	bool(*m_Condition)();
	StateTransitionCondition(bool(*func)()) : m_Condition(func) {};
};


struct State
{
	std::string m_Name{ "" };
	void(*m_Exit)() {nullptr};
	void(*m_Enter)() { nullptr };
	std::vector<std::pair<State, StateTransitionCondition>> m_Transitions;
	State() {};
	State(std::string const& name) : m_Name(name) {};
};


class IStateMachine
{
public:
	virtual void Update() = 0;
	virtual void Destroy() = 0;
	virtual void ProcessConditions() = 0;
	virtual void AddState(State const& newState);

	IStateMachine();
	~IStateMachine();
protected:
	virtual void Init();
	State m_CurrentState;
	std::vector<State> m_States;


};


#endif
