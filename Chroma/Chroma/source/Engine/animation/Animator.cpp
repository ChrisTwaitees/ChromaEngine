#include "Animator.h"
#include <model/SkinnedMesh.h>
#include <scene/Scene.h>
#include <component/CharacterControllerComponent.h>

void Animator::PlayTake(std::string const& takeName, float const& normalizedTime)
{

	// check for valid take
	if (m_Takes.find(takeName) != m_Takes.end())
	{
		float frameNum = CalculateFrameNumber(takeName, normalizedTime);
		glm::mat4 identity(1.0);
		ApplyAnimJointHierarchy(m_Skeleton->GetRootJointID(), m_Takes.at(takeName).m_KeyFrames, identity, frameNum);
	}
	else
	{
		CHROMA_WARN("NO TAKE : {0} FOUND, CANNOT APPLY ANIMATION", m_CurrentTake);
		m_Skeleton->SetToBindPose();
		return;
	}
	



}

float Animator::CalculateFrameNumber(std::string const& takeName, float const& normalizedTime)
{
	return Chroma::Math::Remap(normalizedTime, 0.0, 1.0, 0.0, (float)m_Takes.at(takeName).m_NumFrames);
}

void Animator::ApplyAnimJointHierarchy(int const& jointID, KeyFrames& keyFrames, glm::mat4& parentTransform, float const& frameNum)
{
	// Get Animated Local Transform
	glm::mat4 LocalAnimatedTransform =  GetJointMat4AtKeyFrameTime(m_Skeleton->GetJointName(jointID), keyFrames, frameNum);
	// Convert to Model Space Transform
	glm::mat4 ModelAnimatedTransform = parentTransform * LocalAnimatedTransform;
	// Calculate Accumatively
	for (int const& childJointID : m_Skeleton->GetJointPtr(jointID)->m_ChildJointIDs)
		ApplyAnimJointHierarchy(childJointID, keyFrames, ModelAnimatedTransform, frameNum);
	// Once child joint transforms have been calculated 
	m_Skeleton->GetJointPtr(jointID)->m_ModelSpaceTransform =  ModelAnimatedTransform;
}

glm::mat4 Animator::GetJointMat4AtKeyFrameTime(std::string const& jointName, KeyFrames& keyFrames, float frameNum)
{
	// Fetch KeyFrame and Calculate Transform at Time stamp
	if (keyFrames.find(jointName) != keyFrames.end())
	{
		return JointTransformToMat4(GetJointTransformAtKeyFrameTime(keyFrames.at(jointName), frameNum));
	}
	else
	{
		// No Keyframe found for joint, return identity matrix
		return glm::mat4(1.0);
	}
}

JointTransform Animator::GetJointTransformAtKeyFrameTime(KeyFrame& keyFrame, float const& timeStamp)
{
	float nearestCurrent{ 0.0 };
	float nearestNext{ 0.0 };

	std::map<float, JointTransform>::iterator it = keyFrame.m_JointTransforms.begin();
	// TODO : implement better search nearest frame algorithm
	// iterate through framenumbers stop as soon as next framenumber higher than current search
	while (it != keyFrame.m_JointTransforms.end())
	{
		// joint transforms : map<timestamp, jointTransform>
		if (std::next(it)->first > timeStamp)
		{
			nearestCurrent = it->first;
			nearestNext = std::next(it)->first;
			break;
		}
		++it;
	}
	// if current frame practically the same as current time stamp don't bother interpolating
	if (timeStamp - nearestCurrent < std::numeric_limits<float>::epsilon())
	{
		return keyFrame.m_JointTransforms.at(nearestCurrent);
	}
	else
	{
		// if timestamp sitting between frames interpolate jointtransforms
		float interpolateAmount{ (timeStamp - nearestCurrent) / (nearestNext - nearestCurrent) };
		return InterpolateJointTransforms(keyFrame.m_JointTransforms.at(nearestCurrent), keyFrame.m_JointTransforms.at(nearestNext), interpolateAmount);
	}
}

glm::mat4 Animator::JointTransformToMat4(JointTransform const& jointTransform)
{
	glm::mat4 jointToLocal = glm::translate(glm::mat4(1.0), jointTransform.m_Translation);
	jointToLocal = jointToLocal * glm::toMat4(jointTransform.m_Rotation);
	return glm::scale(jointToLocal, jointTransform.m_Scale);
	
}

JointTransform Animator::InterpolateJointTransforms(JointTransform const& from, JointTransform const& to, float const& lerp)
{
	JointTransform interpolated;

	interpolated.m_Translation = glm::mix(from.m_Translation, to.m_Translation, lerp);
	interpolated.m_Rotation = glm::normalize(glm::mix(from.m_Rotation, to.m_Rotation, lerp));
	interpolated.m_Scale = glm::mix(from.m_Scale, to.m_Scale, lerp);

	return interpolated;
}

void Animator::LoadAnimations(std::string const& sourcePath)
{
	// Load takes from source file
	for (Take& take : Chroma::AnimationLoader::LoadAnimations(sourcePath))
	{
		AddTake(take);
		m_CurrentTake = take.m_Name;
	}
}

void Animator::AddTake(Take const& newTake)
{
	m_Takes[newTake.m_Name] = newTake;
}


void Animator::BindSkeleton(IComponent* const& meshComponent)
{
	m_Skeleton = static_cast<SkinnedMesh*>(meshComponent)->GetSkeleton();
}

void Animator::Update()
{
	// check for skeleton
	if (m_Skeleton == nullptr)
	{
		CHROMA_ERROR("ANIMATOR ERROR:: No Skeleton Found, cannot perform animation!");
		return;
	}

	// check if valid take
	// play take
	PlayTake(m_CurrentTake, Chroma::Time::GetLoopingTimeNormalized(m_Takes.at(m_CurrentTake).m_Duration));
}

void Animator::DebugAnimationTake(std::string const& takeName, float const& debugTime)
{
	if (m_Skeleton == nullptr)
	{
		CHROMA_WARN("ANIMATOR ERROR:: No Skeleton Found, cannot perform animation!");
		return;
	}

	PlayTake(takeName, debugTime);
}

void Animator::CompressAnimations()
{
	// Filter out unneeded joints
	for (std::pair<std::string, Take> take : m_Takes)
	{
		Chroma::AnimationCompress::RemoveKeysNotInSkeleton(take.second, m_Skeleton);
	}
}


void Animator::Destroy()
{
	m_CurrentTake = "";
	m_Takes.clear();
}

Animator::Animator()
{
	Init();
}


Animator::~Animator()
{
}

void Animator::Init()
{
	CHROMA_INFO("Animator Initialized");
}
