#include "Animator.h"



void Animator::AddTake(Take const& newTake)
{
	m_Takes[newTake.m_Name] = newTake;
}

void Animator::PlayTake(std::string const& takeName, float const& timeStamp)
{

	if (m_Takes.find(takeName) != m_Takes.end())
	{
		float framenum = glm::mod(timeStamp, m_Takes.at(takeName).m_Duration) * (1.0 / m_Takes.at(takeName).m_Duration) * m_Takes.at(takeName).m_NumFrames;
		ApplyAnimJointHierarchy(m_Skeleton->GetRootJointID(), m_Takes.at(takeName).m_KeyFrames, m_Skeleton->GetRootTransform(), framenum);
	}
	else
	{
		std::cout << "NO TAKE : " << takeName << "FOUND, CANNOT APPLY ANIMATION" << std::endl;
		return;
	}
}

void Animator::ApplyAnimJointHierarchy(int const& jointID, KeyFrames& keyFrames, glm::mat4 const& parentTransform, float const& timeStamp)
{
	// Calculate Local JointTransform at Time
	glm::mat4 localAnimatedJointTransform = parentTransform * GetJointKeyFrameTransformAtTime(m_Skeleton->GetJointName(jointID), keyFrames, timeStamp) * m_Skeleton->GetJointPtr(jointID)->GetModelBindTransform();
	// Create Model Space Anim Transform
	glm::mat4 FinalAnimTransform =  localAnimatedJointTransform * m_Skeleton->GetJointPtr(jointID)->GetModelBindTransform();
	// Set Model Space Anim Transform
	m_Skeleton->GetJointPtr(jointID)->SetFinalTransform(FinalAnimTransform);
	//m_Skeleton->GetRootTransform() * 
	for (int const& childJointID : m_Skeleton->GetJointPtr(jointID)->GetChildJointIDs())
		ApplyAnimJointHierarchy(childJointID, keyFrames, localAnimatedJointTransform, timeStamp);

}

glm::mat4 Animator::GetJointKeyFrameTransformAtTime(std::string const& jointName, KeyFrames& keyFrames, float timeStamp)
{
	// Get Joint's KeyFrame
	if (keyFrames.find(jointName) != keyFrames.end())
	{
		JointTransform animatedJointTransform = CalculateJointTransformAtTime(keyFrames.at(jointName), timeStamp);
		return JointTransformToLocalTransform(animatedJointTransform);
	}
	else
	{
		// No Keyframe found for joint, return identity matrix
		return glm::mat4(1);
	}
	
}


JointTransform Animator::CalculateJointTransformAtTime(KeyFrame& keyFrame, float const& timeStamp)
{
	float nearestCurrent{ 0.0 };
	float nearestNext{ 0.0 };

	std::map<float, JointTransform>::iterator it = keyFrame.m_JointTransforms.begin();

	while (it != keyFrame.m_JointTransforms.end())
	{
		if (std::next(it)->first > timeStamp)
		{
			nearestCurrent = it->first;
			nearestNext = std::next(it)->first;
			break;
		}
		++it;
	}

	if (timeStamp - nearestCurrent  < std::numeric_limits<float>::epsilon())
	{
		return keyFrame.m_JointTransforms.at(nearestCurrent);
	}
	else
	{
		float lerp{ (timeStamp - nearestCurrent) / (nearestNext - nearestCurrent) };
		return InterpolateJointTransforms(keyFrame.m_JointTransforms.at(nearestCurrent), keyFrame.m_JointTransforms.at(nearestNext), lerp);
	}
}

glm::mat4 Animator::JointTransformToLocalTransform(JointTransform& jointTransform)
{
	glm::mat4 jointToLocal = glm::translate(glm::mat4(1), jointTransform.m_Translation);
	jointToLocal = glm::toMat4(jointTransform.m_Rotation) * jointToLocal;
	return glm::scale(jointToLocal, jointTransform.m_Scale);
}


JointTransform Animator::InterpolateJointTransforms(JointTransform const& from, JointTransform const& to, float const& lerp)
{
	JointTransform interpolated;

	interpolated.m_Translation = glm::mix(from.m_Translation, to.m_Translation, lerp);
	interpolated.m_Rotation = glm::slerp(from.m_Rotation, to.m_Rotation, lerp);
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

void Animator::TriggerTimer(float const& duration, float& counter)
{
//	IWorker::AddJob(this->TriggerTimer);
}

void Animator::DoAnimation(Time& time)
{
	if (m_Skeleton == nullptr)
	{
		std::cout << "::ANIMATOR ERROR:: No Skeleton Found, cannot perform animation" << std::endl;
		return;
	}

	PlayTake(m_CurrentTake, time.GetGameTime());
}

Animator::Animator()
{
}


Animator::~Animator()
{
}
