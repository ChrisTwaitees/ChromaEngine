#ifndef _CHROMA_ANIMATOR_H_
#define _CHROMA_ANIMATOR_H_

// stl
#include <common/CoreCommon.h>
// thirdparty
#include <GLFW/glfw3.h>
// chroma
#include <math/Math.h>
#include <common/CoreCommon.h>

#include <resources/AnimationLoader.h>
#include <resources/AnimationCompression.h>

#include <animation/IStateMachine.h>
#include <animation/Skeleton.h>


class IComponent;

typedef std::map<std::string, KeyFrame> KeyFrames;

class Animator
{
public:
	void LoadAnimations(std::string const& sourcePath);

	void BindSkeleton(Skeleton* newSkeleton) { m_Skeleton = newSkeleton; };

	void BindSkeleton(IComponent* const& meshComponent);

	void Update();

	void DebugAnimationTake(std::string const& takeName, float const& debugTime);

	void CompressAnimations();

	inline void SetCharacterControllerComponentUID(UID const& newCharacterControllerComponentUID) { m_CharacterControllerComponentUID = newCharacterControllerComponentUID; }

	inline void SetAnimationComponentUID(UID const& newAnimationComponentUID) { m_AnimationComponentUID = newAnimationComponentUID; }

	void Destroy();

	Animator();
	~Animator();

private:
	// skeleton
	Skeleton* m_Skeleton{ nullptr };

	// state machine
	IStateMachine* m_StateMachine{ nullptr };

	// UIDs to Existing Components
	UID m_CharacterControllerComponentUID;
	UID m_AnimationComponentUID;

	// takes
	std::map<std::string, Take> m_Takes;
	std::string m_CurrentTake;
	void AddTake(Take const& newTake);

	// animation
	void PlayTake(std::string const& takeName, float const& normalizedTime);

	// time
	float CalculateFrameNumber(std::string const& takeName, float const& normalizedTime);

	// joint transforms
	void ApplyAnimJointHierarchy(int const& jointID, KeyFrames& keyFrames, glm::mat4& parentTransform , float const& timeStamp);

	glm::mat4 GetJointMat4AtKeyFrameTime(std::string const& jointName, KeyFrames& keyFrames, float timeStamp);
	JointTransform GetJointTransformAtKeyFrameTime(KeyFrame& keyFrame, float const& timeStamp);
	JointTransform InterpolateJointTransforms(JointTransform const& from, JointTransform const& to, float const& lerp);
	glm::mat4 JointTransformToMat4(JointTransform const& jointTransform);

};

#endif 