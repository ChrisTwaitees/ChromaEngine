#ifndef _CHROMA_STATE_MACHINE_H_
#define _CHROMA_STATE_MACHINE_H_

//common
#include <common/PrecompiledHeader.h>

class IStateMachine
{
protected:
	void Init();


public:
	IStateMachine();
	~IStateMachine();
};


#endif
