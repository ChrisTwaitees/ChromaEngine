#ifndef _CHROMA_ANIMATION_COMPONENT_H_
#define _CHROMA_ANIMATION_COMPONENT_H_

//common
#include <common/PrecompiledHeader.h>
// chroma
#include <component/IComponent.h>
#include <animation/Animator.h>
#include <time/Time.h>

class AnimationComponent : public IComponent
{
public:
	void Init() override;
	void Update()override;
	void Destroy() override;
	void Serialize(ISerializer*& serializer ) override;

	std::string GetTypeString() const override { return "AnimationComponent"; }

	void AddAnimator(Animator& newAnimator);
	void SetCharacterControllerComponentUID(UID const& newCharacterControllerComponentUID);

	AnimationComponent();
	~AnimationComponent();

private:
	std::vector<Animator> m_Animators;
	void UpdateDebug(std::string const& debugAnimClipName, float const& debugTime);
};

#endif