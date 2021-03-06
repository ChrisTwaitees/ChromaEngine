#ifndef _CHROMA_ANIMATION_COMPONENT_H_
#define _CHROMA_ANIMATION_COMPONENT_H_

//common
#include <common/PrecompiledHeader.h>
// chroma
#include <component/IComponent.h>
#include <animation/Animator.h>
#include <animation/AnimationStateMachine.h>
#include <time/Time.h>

namespace Chroma
{

	class AnimationComponent : public IComponent
	{
	public:
		void Init() override;
		void OnUpdate() override;
		void Destroy() override;
		void Serialize(ISerializer*& serializer ) override;

		void SetAnimator(Animator& newAnimator);
		void SetAnimationStateMachine(AnimationStateMachine*& newAnimationStateMachine);
		AnimationStateMachine*& GetAnimationStateMachine() { return m_AnimationStateMachine; };

		Animator& GetAnimator() { return m_Animator; };

		AnimationComponent();
		~AnimationComponent();

	private:
		Animator m_Animator;
		AnimationStateMachine* m_AnimationStateMachine{nullptr};
		void UpdateDebug(std::string const& debugAnimClipName, float const& debugTime);
	};

}
#endif