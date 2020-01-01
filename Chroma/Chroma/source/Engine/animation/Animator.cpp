#include "Animator.h"


void Animator::PlayTake(std::string const& takeName, float const& normalizedTime)
{

	if (m_Takes.find(takeName) != m_Takes.end())
	{
		float frameNum = CalculateFrameNumber(takeName, normalizedTime);
		ApplyAnimJointHierarchy(m_Skeleton->GetRootJointID(), m_Takes.at(takeName).m_KeyFrames, glm::mat4(1.0), frameNum);
	}
	else
	{
		CHROMA_WARN("NO TAKE : {0} FOUND, CANNOT APPLY ANIMATION", takeName);
		return;
	}
}

float Animator::CalculateFrameNumber(std::string const& takeName, float const& normalizedTime)
{
	return  Chroma::Time::GetLoopingTimeNormalized(m_Takes.at(takeName).m_Duration) * m_Takes.at(takeName).m_NumFrames;
}

void Animator::ApplyAnimJointHierarchy(int const& jointID, KeyFrames& keyFrames, glm::mat4 const& parentTransform, float const& frameNum)
{
	// Create Model Space Anim Transform
	glm::mat4 localAnimatedTransform =  GetJointMat4AtKeyFrameTime(m_Skeleton->GetJointName(jointID), keyFrames, frameNum);
	//glm::mat4 ModelAnimatedTransform = m_Skeleton->GetJoint(jointID).GetLocalBindOffsetTransform() * parentTransform *  localAnimatedTransform;
	glm::mat4 ModelAnimatedTransform = parentTransform * localAnimatedTransform;
	
	m_Skeleton->GetJointPtr(jointID)->SetModelSpaceTransform(ModelAnimatedTransform);

	for (int const& childJointID : m_Skeleton->GetJointPtr(jointID)->GetChildJointIDs())
		ApplyAnimJointHierarchy(childJointID, keyFrames, ModelAnimatedTransform, frameNum);

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
	jointToLocal = glm::toMat4(jointTransform.m_Rotation) * jointToLocal;
	return glm::scale(jointToLocal, jointTransform.m_Scale);
	
}

JointTransform Animator::InterpolateJointTransforms(JointTransform const& from, JointTransform const& to, float const& lerp)
{
	JointTransform interpolated;

	interpolated.m_Translation = glm::mix(from.m_Translation, to.m_Translation, lerp);
	interpolated.m_Rotation = glm::mix(from.m_Rotation, to.m_Rotation, lerp);
	interpolated.m_Scale = glm::mix(from.m_Scale, to.m_Scale, lerp);

	return interpolated;
}

void Animator::LoadAnimations(std::string const& sourcePath)
{
	for (Take const& take : AnimationLoader::LoadAnimations(sourcePath))
	{
		AddTake(take);
		m_CurrentTake = take.m_Name;
	}
}

void Animator::AddTake(Take const& newTake)
{
	m_Takes[newTake.m_Name] = newTake;
}


void Animator::DoAnimation()
{
	if (m_Skeleton == nullptr)
	{
		CHROMA_WARN("ANIMATOR ERROR:: No Skeleton Found, cannot perform animation!");
		return;
	}



	PlayTake(m_CurrentTake, 0.5);
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




Animator::Animator()
{
}


Animator::~Animator()
{
}
