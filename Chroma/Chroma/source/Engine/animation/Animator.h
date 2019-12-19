#ifndef _CHROMA_ANIMATOR_
#define _CHROMA_ANIMATOR_

// stl
#include <vector>
// thirdparty
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// chroma
#include <animation/Animation.h>
#include <animation/Skeleton.h>
#include <time/ChromaTime.h>
#include <worker/IWorker.h>


class Animator
{
	std::vector<Animation> m_Animations;
	Skeleton* m_Skeleton{ nullptr };
	
public:

	void AddAnimation(Animation const& newAnimation) { m_Animations.push_back(newAnimation); };
	void TriggerTimer(float const& duration, float& counter);

	void BindSkeleton(Skeleton* newSkeleton) { m_Skeleton = newSkeleton; };

	void DoAnimation(Time& time);

	Animator();
	~Animator();
};

#endif _CHROMA_ANIMATOR_