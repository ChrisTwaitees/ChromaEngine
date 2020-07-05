#ifndef _CHROMA_STATEMACHINE_COMPONENT_H_
#define _CHROMA_STATEMACHINE_COMPONENT_H_

#include <component/IComponent.h>
#include <statemachine/IStateMachine.h>


namespace Chroma
{
	class StateMachineComponent : public IComponent , public IStateMachine
	{
	public:

		// components
		virtual void Init() override;
		virtual void OnUpdate() override;
		virtual void Destroy() override;
		virtual void Serialize(ISerializer*& serializer) override;

		// funcs
		virtual void ProcessConditions() {};
	};
}

#endif