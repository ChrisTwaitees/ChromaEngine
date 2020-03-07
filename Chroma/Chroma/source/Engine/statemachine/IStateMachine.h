#ifndef _CHROMA_STATE_MACHINE_H_
#define _CHROMA_STATE_MACHINE_H_

//common
#include <common/PrecompiledHeader.h>



struct State
{
	std::string m_Name{ "" };
	void* m_TransitionFunc{ nullptr };
};


class IStateMachine
{
public:
	void Update();

	void AddState(State const& newState);

	IStateMachine();
	~IStateMachine();
protected:
	void Init();
	State m_CurrentState;
	std::vector<State> m_States;


};


#endif
