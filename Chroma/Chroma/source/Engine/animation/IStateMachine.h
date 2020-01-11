#ifndef _CHROMA_STATE_MACHINE_H_
#define _CHROMA_STATE_MACHINE_H_

// chroma
#include <common/CoreCommon.h>

class IStateMachine
{
protected:
	void Init();


public:
	IStateMachine();
	~IStateMachine();
};


#endif
