#ifndef _CHROMA_ANIMATION_COMPONENT_H_
#define _CHROMA_ANIMATION_COMPONENT_H_

// stl
#include <vector>
// chroma
#include <component/IComponent.h>
#include <animation/Animator.h>

class AnimationComponent : public IComponent
{
	std::vector<Animator> m_Animators;

public:

	void AddAnimator(Animator const& newAnimator) { m_Animators.push_back(newAnimator); };

	void ProcessAnimators();
	AnimationComponent();
	~AnimationComponent();
};

#endif