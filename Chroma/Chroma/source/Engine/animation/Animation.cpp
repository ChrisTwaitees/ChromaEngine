#include "Animation.h"



void Animation::LoadAnimation(std::string sourcePath)
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
		ProcessAnimation(scene, scene->mRootNode);
	}
}

void Animation::ProcessAnimation(const aiScene* scene, aiNode* rootNode)
{
	// collect animations
	for (unsigned int i = 0; i < scene->mNumAnimations ; i++)
	{
		// storing each animation as a "take"
		AnimTake newAnimTake;
		aiAnimation* aiAnim = scene->mAnimations[i];

		// AnimTake data
		newAnimTake.m_Name = aiAnim->mName.C_Str();
		newAnimTake.m_NumFrames = aiAnim->mDuration;
		newAnimTake.m_FPS = aiAnim->mTicksPerSecond;
		newAnimTake.m_Duration = 1.0 / newAnimTake.m_FPS * newAnimTake.m_NumFrames;
		newAnimTake.m_RootTransform = AIToGLM(rootNode->mTransformation);

		std::cout << "Animation Name : " << newAnimTake.m_Name << std::endl;
		std::cout << "Animation NumFrames : " << newAnimTake.m_NumFrames << std::endl;
		std::cout << "Animation FPS : " << newAnimTake.m_FPS << std::endl;
		std::cout << "Animation Duration : " << newAnimTake.m_Duration << std::endl;

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
				// position , framenumber
				std::pair<glm::vec3, float> positionAtTime = std::make_pair(AItoGLM(aiAnimNode->mPositionKeys[a].mValue), aiAnimNode->mPositionKeys[a].mTime);
				newKeyFrame.m_Positions.push_back(positionAtTime);
			}

			// Rotations
			for (unsigned int a = 0; a < aiAnimNode->mNumRotationKeys; a++)
			{
				// rotation , framenumber
				std::pair<glm::quat, float> rotationAtTime = std::make_pair(AItoGLM(aiAnimNode->mRotationKeys[a].mValue), aiAnimNode->mRotationKeys[a].mTime);
				newKeyFrame.m_Rotations.push_back(rotationAtTime);
			}

			// Scales
			for (unsigned int a = 0; a < aiAnimNode->mNumScalingKeys; a++)
			{
				// scale , framenumber
				std::pair<glm::vec3, float> scaleAtTime = std::make_pair(AItoGLM(aiAnimNode->mScalingKeys[a].mValue), aiAnimNode->mScalingKeys[a].mTime);
				newKeyFrame.m_Scales.push_back(scaleAtTime);
			}

			// add keyframe to take
			newAnimTake.m_KeyFrames.push_back(newKeyFrame);
		
		}
		// add to takes
		m_AnimTakes.push_back(newAnimTake);
	}
}

Animation::Animation(std::string sourcePath)
{
	LoadAnimation(sourcePath);
}

Animation::~Animation()
{
}
