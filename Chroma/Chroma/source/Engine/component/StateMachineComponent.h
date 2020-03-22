#ifndef _CHROMA_STATEMACHINE_COMPONENT_H_
#define _CHROMA_STATEMACHINE_COMPONENT_H_

#include <component/IComponent.h>
#include <statemachine/IStateMachine.h>



class StateMachineComponent : public IComponent , public IStateMachine
{
public:

	// components
	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;
	virtual void Serialize(ISerializer*& serializer) override;
	virtual std::string GetTypeString() const { return "StateMachineComponent"; };

	// funcs
	virtual void ProcessConditions() {};

protected:
	// funcs

};

#endif