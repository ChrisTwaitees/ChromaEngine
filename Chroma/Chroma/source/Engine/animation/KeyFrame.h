#ifndef _CHROMA_KEYFRAME_
#define _CHROMA_KEYFRAME_

#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct JointTransform
{
	glm::vec3 position;
	glm::quat rotation;
};

class KeyFrame
{
	std::vector<JointTransform> m_jointTransforms;
	float m_timeStamp;

	
public:
	void setJointTransforms(std::vector<JointTransform> jointTransforms) { m_jointTransforms = jointTransforms; };

	float getTimeStamp() {return m_timeStamp; };
	std::vector<JointTransform> getJointTransforms() { return m_jointTransforms; };
	KeyFrame();
	~KeyFrame();
};

static KeyFrame lerpKeyFrames(KeyFrame sourceKeyFrame, KeyFrame targetKeyFrame, float bias)
{
	KeyFrame newKeyFrame;
	std::vector<JointTransform> lerpedJointTransforms;

	if (sourceKeyFrame.getJointTransforms().size() == targetKeyFrame.getJointTransforms().size())
		for (int i = 0 ; i < sourceKeyFrame.getJointTransforms().size(); i++)
		{
			JointTransform lerpedTransform;
			lerpedTransform.position = glm::mix(sourceKeyFrame.getJointTransforms()[i].position, targetKeyFrame.getJointTransforms()[i].position, bias);
			lerpedTransform.rotation = glm::slerp(sourceKeyFrame.getJointTransforms()[i].rotation, targetKeyFrame.getJointTransforms()[i].rotation, bias);
			lerpedJointTransforms.push_back(lerpedTransform);
		}
	else
		std::cout << "KeyFrames incompatible" << std::endl;

	newKeyFrame.setJointTransforms(lerpedJointTransforms);
	return newKeyFrame;
};

#endif