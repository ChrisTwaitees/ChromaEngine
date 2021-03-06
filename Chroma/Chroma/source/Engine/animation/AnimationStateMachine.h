#ifndef CHROMA_ANIMATIONSTATEMACHINE_H
#define CHROMA_ANIMATIONSTATEMACHINE_H

#include <uid/UID.h>
#include <component/StateMachineComponent.h>


namespace Chroma
{
	class CharacterControllerComponent;
	class Animator;
	struct Take;
	typedef std::pair<Take, float> TakeTime;

	struct AnimStateTransitionCondition
	{
		bool(*m_Condition)(CharacterControllerComponent* charController);
		AnimStateTransitionCondition(bool(*func)(CharacterControllerComponent*)) : m_Condition(func) {};
	};


	struct AnimState : public State
	{
		bool m_IsLooping{false};
		float m_TransitionTime{ 0.2f };
		float m_CurrentTime{ 0.0f };

		std::vector<std::pair<AnimState, AnimStateTransitionCondition>>* m_Transitions{ new std::vector<std::pair<AnimState, AnimStateTransitionCondition>> };

		AnimState() {};
		AnimState(std::string const& takeName) { m_Name = takeName; };
	};


	class AnimationStateMachine : public StateMachineComponent
	{
	public:
		void OnUpdate() override;
		void Destroy() override;
		void ProcessConditions() override {};
	
		virtual void ProcessAnimator() {};
		virtual void ProcessAnimStates() {};
		virtual void TranstionTo(AnimState const& newState);

		void SetAnimationComponentUID(UID const& animcompUID) { m_AnimationComponentUID = animcompUID; };

		AnimationStateMachine() {};
		~AnimationStateMachine() {};

	protected:

		CharacterControllerComponent* GetCharacterController();
		Animator& GetAnimator();
		Take& GetTake(std::string const& takeName);
		UID m_AnimationComponentUID;

		std::vector<AnimState> m_States;
		AnimState m_CurrentState;
		AnimState m_PreviousState;

		float m_TransitionTimer{ 1.0f };
		bool m_IsTransitioning{ false };
	
	};
}

#endif