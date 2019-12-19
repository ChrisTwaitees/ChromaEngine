#ifndef _CHROMA_ANIMATOR_
#define _CHROMA_ANIMATOR_

// stl
#include <vector>
// thirdparty
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// chroma
#include <animation/AnimationLoader.h>
#include <animation/Skeleton.h>
#include <time/ChromaTime.h>
#include <worker/IWorker.h>


class Animator
{
	std::vector<Take> m_Takes;
	Skeleton* m_Skeleton{ nullptr };
	void AddTake(Take const& newTake) { m_Takes.push_back(newTake); };
	
public:
	void LoadAnimations(std::string const& sourcePath);

	void TriggerTimer(float const& duration, float& counter);

	void BindSkeleton(Skeleton* newSkeleton) { m_Skeleton = newSkeleton; };

	void DoAnimation(Time& time);

	Animator();
	~Animator();
};

#endif _CHROMA_ANIMATOR_