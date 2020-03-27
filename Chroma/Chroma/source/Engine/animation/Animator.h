#ifndef _CHROMA_ANIMATOR_H_
#define _CHROMA_ANIMATOR_H_

//common
#include <common/PrecompiledHeader.h>

// stl
#include <common/CoreCommon.h>

// chroma
#include <math/Math.h>

#include <resources/AnimationLoader.h>
#include <resources/AnimationCompression.h>

#include <animation/Skeleton.h>


class IComponent;

typedef std::map<std::string, KeyFrame> KeyFrames;
typedef std::pair<float, Take> TakeState;

class Animator
{
public:
	void Update();
	void Destroy();

	// animation
	void PlayTake(std::string const& takeName, float const& normalizedTime);

	void LerpTakes(TakeState const& stateFrom, TakeState const& stateTo);

	void LoadAnimations(std::string const& sourcePath);

	void BindSkeleton(Skeleton* newSkeleton) { m_Skeleton = newSkeleton; };

	void BindSkeleton(IComponent* const& meshComponent);

	void DebugAnimationTake(std::string const& takeName, float const& debugTime);

	void CompressAnimations();

	inline void SetCharacterControllerComponentUID(UID const& newCharacterControllerComponentUID) { m_CharacterControllerComponentUID = newCharacterControllerComponentUID; }

	inline void SetAnimationComponentUID(UID const& newAnimationComponentUID) { m_AnimationComponentUID = newAnimationComponentUID; }

	std::map<std::string, Take>& GetTakes() { return m_Takes; };
	Take& GetTake(std::string const& takeName);
	
	Animator();
	~Animator();

private:
	// init
	void Init();

	// skeleton
	Skeleton* m_Skeleton{ nullptr };

	// UIDs to Existing Components
	UID m_CharacterControllerComponentUID;
	UID m_AnimationComponentUID;

	// takes
	std::map<std::string, Take> m_Takes;
	std::string m_CurrentTake;
	void AddTake(Take const& newTake);

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