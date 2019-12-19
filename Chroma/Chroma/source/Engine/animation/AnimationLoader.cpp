#include "AnimationLoader.h"


void AssimpLoadAnimation(std::string const& sourcePath, std::vector<Take>& takes)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(sourcePath, NULL);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	else if (!scene->HasAnimations())
	{
		std::cout << "ERROR::ASSIMP:: " << sourcePath << ". Contains no recognized Animation Data." << std::endl;
		return;
	}
	else
	{
		// process animation
		ProcessTakes(scene, scene->mRootNode, takes);
	}
}

void ProcessTakes(const aiScene* scene, aiNode* rootNode, std::vector<Take>& takes)
{
	// collect animations
	for (unsigned int i = 0; i < scene->mNumAnimations ; i++)
	{
		// storing each animation as a "take"
		Take newTake;
		aiAnimation* aiAnim = scene->mAnimations[i];

		// AnimTake data
		newTake.m_Name = aiAnim->mName.C_Str();
		newTake.m_NumFrames = aiAnim->mDuration;
		newTake.m_FPS = aiAnim->mTicksPerSecond;
		newTake.m_Duration = 1.0 / newTake.m_FPS * newTake.m_NumFrames;

		std::cout << "Animation Name : " << newTake.m_Name << std::endl;
		std::cout << "Animation NumFrames : " << newTake.m_NumFrames << std::endl;
		std::cout << "Animation FPS : " << newTake.m_FPS << std::endl;
		std::cout << "Animation Duration : " << newTake.m_Duration << std::endl;

		// storing keyframes in take
		for (unsigned int j = 0; j < aiAnim->mNumChannels; j++)
		{
			// aiNodeAnim/KeyFrames/Channels contain Joint's transforms key data of entirety of animation
			aiNodeAnim* aiAnimNode = aiAnim->mChannels[j];
			KeyFrame newKeyFrame;

			// Name and joint id
			newKeyFrame.m_JointID = j;
			newKeyFrame.m_JointName = aiAnimNode->mNodeName.C_Str();

			// Positions
			for (unsigned int a = 0; a < aiAnimNode->mNumPositionKeys; a++)
			{
				float timeStamp = aiAnimNode->mPositionKeys[a].mTime;
				glm::vec3 translation = AItoGLM(aiAnimNode->mPositionKeys[a].mValue);
				// position , framenumber
				if (newKeyFrame.m_JointTransforms.find(timeStamp) != newKeyFrame.m_JointTransforms.end())
				{
					newKeyFrame.m_JointTransforms.find(timeStamp)->second.m_Translation = translation;
				}
				else
				{
					JointTransform newJointTransform;
					newJointTransform.m_Translation = translation;
					newKeyFrame.m_JointTransforms[timeStamp] = newJointTransform;
				}
			}

			// Rotations
			for (unsigned int a = 0; a < aiAnimNode->mNumRotationKeys; a++)
			{
				float timeStamp = aiAnimNode->mRotationKeys[a].mTime;
				glm::quat rotation = AItoGLM(aiAnimNode->mRotationKeys[a].mValue);
				// rotation , framenumber
				if (newKeyFrame.m_JointTransforms.find(timeStamp) != newKeyFrame.m_JointTransforms.end())
				{
					newKeyFrame.m_JointTransforms.find(timeStamp)->second.m_Rotation = rotation;
				}
				else
				{
					JointTransform newJointTransform;
					newJointTransform.m_Rotation = rotation;
					newKeyFrame.m_JointTransforms[timeStamp] = newJointTransform;
				}
			}

			// Scales
			for (unsigned int a = 0; a < aiAnimNode->mNumScalingKeys; a++)
			{
				float timeStamp = aiAnimNode->mScalingKeys[a].mTime;
				glm::vec3 scale = AItoGLM(aiAnimNode->mScalingKeys[a].mValue);
				// scale , framenumber
				if (newKeyFrame.m_JointTransforms.find(timeStamp) != newKeyFrame.m_JointTransforms.end())
				{
					newKeyFrame.m_JointTransforms.find(timeStamp)->second.m_Scale = scale;
				}
				else
				{
					JointTransform newJointTransform;
					newJointTransform.m_Scale = scale;
					newKeyFrame.m_JointTransforms[timeStamp] = newJointTransform;
				}
			}

			// add keyframe to take
			newTake.m_KeyFrames.push_back(newKeyFrame);
		
		}
		// add to takes
		takes.push_back(newTake);
	}
}


std::vector<Take> AnimationLoader::LoadAnimations(std::string sourcePath)
{
	std::vector<Take> newTakes;
	AssimpLoadAnimation(sourcePath, newTakes);
	return newTakes;
}
