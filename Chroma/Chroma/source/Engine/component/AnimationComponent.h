#ifndef _CHROMA_ANIMATION_COMPONENT_H_
#define _CHROMA_ANIMATION_COMPONENT_H_

// stl
#include <vector>
// chroma
#include <component/IComponent.h>
#include <animation/Animator.h>
#include <time/Time.h>

class AnimationComponent : public IComponent
{
	std::vector<Animator> m_Animators;
	void UpdateDebug(std::string const& debugAnimClipName, float const& debugTime);

public:
	virtual void Init() override;
	virtual void Update()override;
	virtual void Destroy() override;

	void AddAnimator(Animator& newAnimator);
	void SetCharacterControllerComponentUID(UID const& newCharacterControllerComponentUID);

	AnimationComponent();
	~AnimationComponent();
};

#endif