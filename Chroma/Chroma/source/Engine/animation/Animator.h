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



namespace Chroma
{
	class AnimationComponent;

	typedef std::map<std::string, KeyFrame> KeyFrameArray;
	typedef std::pair<std::string, float> TakeNameTime;

	class Animator
	{
	public:
		void Update();
		void Destroy();

		// animation
		void PlayTake(std::string const& takeName, float const& normalizedTime);

		void LerpTakes(TakeNameTime const& stateFrom, TakeNameTime const& stateTo, float const& lerpAmount);

		void LoadAnimations(std::string const& sourcePath);

		void BindSkeleton(Skeleton* newSkeleton) { m_Skeleton = newSkeleton; };

		void BindSkeleton(IComponent* const& meshComponent);

		void DebugAnimationTake(std::string const& takeName, float const& debugTime);

		void CompressAnimations();

		inline void SetAnimationComponentUID(UID const& newAnimationComponentUID) { m_AnimationComponentUID = newAnimationComponentUID; }
		inline Skeleton* GetSkeleton() { return m_Skeleton; };

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
		UID m_AnimationComponentUID;
	
		// Get AnimationComponent
		AnimationComponent* GetAnimationComponent();

		// takes
		std::map<std::string, Take> m_Takes;
		std::string m_CurrentTake;
		void AddTake(Take const& newTake);

		// time
		float CalculateFrameNumber(std::string const& takeName, float const& normalizedTime);

		// joint transforms
		glm::mat4 m_Identity{ glm::mat4(1.0f) };

		void ApplyAnimJointHierarchy(int const& jointID, KeyFrameArray& keyFrames, glm::mat4& parentTransform , float const& frameNum);
		void ApplyAnimJointHierarchyLerped(int const& jointID, KeyFrameArray& fromKeyFrames, float const& fromFrameNum, KeyFrameArray& toKeyFrame, float const& toFrameNum, float const& lerpAmount, glm::mat4 const& parentTransform);

		JointTransform GetJointTransformAtKeyFrameTime(KeyFrame& keyFrame, float const& frameNum);
		JointTransform GetJointTransformAtKeyFrameArrayTime(std::string const& jointName, KeyFrameArray& keyFrames, float const& frameNum);
		JointTransform InterpolateJointTransforms(JointTransform const& from, JointTransform const& to, float const& lerp);

		glm::mat4 GetJointMat4AtKeyFrameTime(std::string const& jointName, KeyFrameArray& keyFrames, float frameNum);
		glm::mat4 JointTransformToMat4(JointTransform const& jointTransform);

	};
}

#endif 