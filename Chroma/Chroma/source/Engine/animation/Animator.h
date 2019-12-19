#ifndef _CHROMA_ANIMATOR_
#define _CHROMA_ANIMATOR_

// stl
#include <vector>
// thirdparty
#include <GLFW/glfw3.h>
#include <worker/IWorker.h>
// chroma
#include <animation/Animation.h>


class Animator
{
	std::vector<Animation> m_Animations;
	
public:

	void AddAnimation(Animation const& newAnimation) { m_Animations.push_back(newAnimation); };
	void TriggerTimer(float const& duration, float& counter);

	void DoAnimation();

	Animator();
	~Animator();
};

#endif _CHROMA_ANIMATOR_