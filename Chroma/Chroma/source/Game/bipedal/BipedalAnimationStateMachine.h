#ifndef _CHROMA_BIPEDAL_ANIMFSM_H_
#define _CHROMA_BIPEDAL_ANIMFSM_H_

#include <animation/AnimationStateMachine.h>


class BipedalAnimationStateMachine : public AnimationStateMachine
{
public:
	void Update() override;
	void Destroy() override;
	void ProcessConditions() override;
	void ProcessAnimStates() override;
	void ProcessAnimator() override;


	void TranstionTo(AnimState const& newState) override;

	BipedalAnimationStateMachine() { Init(); };
	~BipedalAnimationStateMachine() {};
protected:
	void Init() override;

};

#endif