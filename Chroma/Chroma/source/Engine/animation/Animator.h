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
	std::map<std::string, Take> m_Takes;
	std::string m_CurrentTake;

	Skeleton* m_Skeleton{ nullptr };
	void AddTake(Take const& newTake);

	void PlayTake(std::string const& takeName, float const& timeStamp);

	JointTransform CalculateJointTransformAtTime(KeyFrame& keyFrame, float const& timeStamp);
	glm::mat4 JointTransformToLocalTransform(JointTransform& jointTransform);

	KeyFrame GetKeyFrame(std::string const& jointName, std::vector<KeyFrame> keyFrames);

	bool GetJointHasKeys(std::string const& jointName, std::vector<KeyFrame> keyFrames);
	void ApplyAnimJointHierarchy(int const& jointID, std::vector<KeyFrame>& keyFrames, glm::mat4 const& parentTransform , float const& timeStamp);

	JointTransform InterpolateJointTransforms(JointTransform const& from, JointTransform const& to, float const& lerp);


public:
	void LoadAnimations(std::string const& sourcePath);

	void TriggerTimer(float const& duration, float& counter);

	void BindSkeleton(Skeleton* newSkeleton) { m_Skeleton = newSkeleton; };

	void DoAnimation(Time& time);

	Animator();
	~Animator();
};

#endif _CHROMA_ANIMATOR_