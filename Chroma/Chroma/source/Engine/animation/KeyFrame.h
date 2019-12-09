#ifndef _CHROMA_KEYFRAME_
#define _CHROMA_KEYFRAME_

#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct JointTransform
{
	glm::vec3 m_Position{ 0.0 };
	glm::quat m_Rotation{ 0.0, 0.0, 0.0, 0.0 };
};

class KeyFrame
{
	std::vector<JointTransform> m_JointTransforms;
	float m_TimeStamp{ 0.0 };

public:

	// Accessors
	float GetTimeStamp() {return m_TimeStamp; };
	std::vector<JointTransform> GetJointTransforms() { return m_JointTransforms; };

	void SetJointTransforms(std::vector<JointTransform> jointTransforms) { m_JointTransforms = jointTransforms; };

	KeyFrame();
	~KeyFrame();
};

static KeyFrame lerpKeyFrames(KeyFrame sourceKeyFrame, KeyFrame targetKeyFrame, float bias)
{
	KeyFrame newKeyFrame;
	std::vector<JointTransform> lerpedJointTransforms;

	if (sourceKeyFrame.GetJointTransforms().size() == targetKeyFrame.GetJointTransforms().size())
		for (int i = 0 ; i < sourceKeyFrame.GetJointTransforms().size(); i++)
		{
			JointTransform lerpedTransform;
			lerpedTransform.m_Position = glm::mix(sourceKeyFrame.GetJointTransforms()[i].m_Position, targetKeyFrame.GetJointTransforms()[i].m_Position, bias);
			lerpedTransform.m_Rotation = glm::slerp(sourceKeyFrame.GetJointTransforms()[i].m_Rotation, targetKeyFrame.GetJointTransforms()[i].m_Rotation, bias);
			lerpedJointTransforms.push_back(lerpedTransform);
		}
	else
		std::cout << "KeyFrames incompatible" << std::endl;

	newKeyFrame.SetJointTransforms(lerpedJointTransforms);
	return newKeyFrame;
};

#endif