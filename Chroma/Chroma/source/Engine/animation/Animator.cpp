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
		ApplyAnimJointHierarchy(m_Skeleton->GetRootJointID(), m_Takes.at(takeName).m_KeyFrames, m_Identity, frameNum);
	}
	else
	{
		CHROMA_ERROR("NO TAKE : {0} FOUND, CANNOT APPLY ANIMATION", m_CurrentTake);
		m_Skeleton->SetToBindPose();
		return;
	}

}


void Animator::LerpTakes(TakeNameTime const& stateFrom, TakeNameTime const& stateTo, float const& lerpAmount)
{
	CHROMA_INFO("Transitioning from State : {0} to : {1}", stateFrom.first, stateTo.first);
	CHROMA_INFO("Transition timing from   : {0} to : {1}", stateFrom.second, stateTo.second);

	// check takes are valid
	if (m_Takes.find(stateFrom.first) != m_Takes.end() && m_Takes.find(stateTo.first) != m_Takes.end())
	{
		// frame numbers
		float fromFrameNumber = CalculateFrameNumber(stateFrom.first, stateFrom.second);
		float toFrameNumber = CalculateFrameNumber(stateTo.first, stateTo.second);

		// lerping
		ApplyAnimJointHierarchyLerped(m_Skeleton->GetRootJointID(), m_Takes.at(stateFrom.first).m_KeyFrames, fromFrameNumber,
			m_Takes.at(stateTo.first).m_KeyFrames, toFrameNumber, lerpAmount, m_Identity);
		
	}
	else
	{
		CHROMA_ERROR("NO TAKE : {0} FOUND, CANNOT APPLY ANIMATION", m_CurrentTake);
		m_Skeleton->SetToBindPose();
		return;
	}
}


float Animator::CalculateFrameNumber(std::string const& takeName, float const& normalizedTime)
{
	// TODO: update framenumber interpolation
	if (normalizedTime == 1.0)
	{
		return Chroma::Math::Remap(0.999, 0.0, 1.0, 0.0, (float)m_Takes.at(takeName).m_NumFrames);
	}
	else
		return Chroma::Math::Remap(normalizedTime, 0.0, 1.0, 0.0, (float)m_Takes.at(takeName).m_NumFrames);
}

void Animator::ApplyAnimJointHierarchy(int const& jointID, KeyFrameArray& keyFrames, glm::mat4& parentTransform, float const& frameNum)
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

void Animator::ApplyAnimJointHierarchyLerped(int const& jointID, KeyFrameArray& fromKeyFrames, float const& fromFrameNum, KeyFrameArray& toKeyFrames, float const& toFrameNum, float const& lerpAmount, glm::mat4 const& parentTransform)
{
	// Get from and to Joint Transforms 
	JointTransform fromJntTrs = GetJointTransformAtKeyFrameArrayTime(m_Skeleton->GetJointName(jointID), fromKeyFrames, fromFrameNum);
	JointTransform toJntTrs = GetJointTransformAtKeyFrameArrayTime(m_Skeleton->GetJointName(jointID), toKeyFrames, toFrameNum);
	// Interpolate Transforms
	JointTransform InterpolatedJntTrs = InterpolateJointTransforms(fromJntTrs, toJntTrs, lerpAmount);
	// Get Animated Local Transform
	glm::mat4 LocalAnimatedTransform = JointTransformToMat4(InterpolatedJntTrs);
	// Convert to Model Space Transform
	glm::mat4 ModelAnimatedTransform = parentTransform * LocalAnimatedTransform;
	// Calculate Accumatively
	for (int const& childJointID : m_Skeleton->GetJointPtr(jointID)->m_ChildJointIDs)
		ApplyAnimJointHierarchyLerped(childJointID, fromKeyFrames, fromFrameNum, toKeyFrames, toFrameNum, lerpAmount, ModelAnimatedTransform);

	// Once child joint transforms have been calculated 
	m_Skeleton->GetJointPtr(jointID)->m_ModelSpaceTransform = ModelAnimatedTransform;

}

glm::mat4 Animator::GetJointMat4AtKeyFrameTime(std::string const& jointName, KeyFrameArray& keyFrames, float frameNum)
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

JointTransform Animator::GetJointTransformAtKeyFrameTime(KeyFrame& keyFrame, float const& frameNum)
{
	float nearestCurrent{ 0.0 };
	float nearestNext{ 0.0 };

	std::map<float, JointTransform>::iterator it = keyFrame.m_JointTransforms.begin();
	// TODO : implement better search nearest frame algorithm
	// iterate through framenumbers stop as soon as next framenumber higher than current search
	while (it != keyFrame.m_JointTransforms.end())
	{
		// joint transforms : map<timestamp, jointTransform>
		if (std::next(it)->first > frameNum)
		{
			nearestCurrent = it->first;
			nearestNext = std::next(it)->first;
			break;
		}
		++it;
	}
	// if current frame practically the same as current time stamp don't bother interpolating
	if (frameNum - nearestCurrent < std::numeric_limits<float>::epsilon())
	{
		return keyFrame.m_JointTransforms.at(nearestCurrent);
	}
	else
	{
		// if timestamp sitting between frames interpolate jointtransforms
		float interpolateAmount{ (frameNum - nearestCurrent) / (nearestNext - nearestCurrent) };
		return InterpolateJointTransforms(keyFrame.m_JointTransforms.at(nearestCurrent), keyFrame.m_JointTransforms.at(nearestNext), interpolateAmount);
	}
}

JointTransform Animator::GetJointTransformAtKeyFrameArrayTime(std::string const& jointName, KeyFrameArray& keyFrames, float const& frameNum)
{
	// Fetch KeyFrame and Calculate Transform at Time stamp
	if (keyFrames.find(jointName) != keyFrames.end())
	{
		return GetJointTransformAtKeyFrameTime(keyFrames.at(jointName), frameNum);
	}
	else
	{
		// No Keyframe found for joint, return empty JointTransform
		return JointTransform();
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

	// play take
	//PlayTake(m_CurrentTake, Chroma::Time::GetLoopingTimeNormalized(m_Takes.at(m_CurrentTake).m_Duration));
}

Take& Animator::GetTake(std::string const& takeName)
{
	if (m_Takes.find(takeName) != m_Takes.end()) 
		return m_Takes.find(takeName)->second;
	else
	{
		CHROMA_ERROR("Cannot find Take : {0}", takeName);
	}
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
