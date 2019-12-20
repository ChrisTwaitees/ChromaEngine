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

			// aiNodeAnim channels correlate to the joint keyed
			// they name however often does not match the joint so we do a simple search 
			// to find the correct joint 
			std::string JointName = GetJointName(aiAnimNode, scene);
			if (JointName.empty())
			{
				std::cout << "Could not find correlating joint for channel : " << aiAnimNode->mNodeName.C_Str() << std::endl;
				continue;
			}
			// if Keyframe for joint does not exist yet, create one
			std::map<std::string, KeyFrame>::iterator it = newTake.m_KeyFrames.find(JointName);
			if (it == newTake.m_KeyFrames.end())
			{
				KeyFrame newKey;
				newKey.m_JointName = JointName;
				newTake.m_KeyFrames.insert(std::make_pair(JointName, newKey));
			}
			
			// Positions
			for (unsigned int a = 0; a < aiAnimNode->mNumPositionKeys; a++)
			{
				float timeStamp = aiAnimNode->mPositionKeys[a].mTime;
				glm::vec3 translation = AItoGLM(aiAnimNode->mPositionKeys[a].mValue);
				// position , framenumber
				if (newTake.m_KeyFrames.at(JointName).m_JointTransforms.find(timeStamp) != newTake.m_KeyFrames.at(JointName).m_JointTransforms.end())
				{
					newTake.m_KeyFrames.at(JointName).m_JointTransforms.find(timeStamp)->second.m_Translation = translation;
				}
				else
				{
					JointTransform newJointTransform;
					newJointTransform.m_Translation = translation;
					newTake.m_KeyFrames.at(JointName).m_JointTransforms[timeStamp] = newJointTransform;
				}
			}

			// Rotations
			for (unsigned int a = 0; a < aiAnimNode->mNumRotationKeys; a++)
			{
				float timeStamp = aiAnimNode->mRotationKeys[a].mTime;
				glm::quat rotation = AItoGLM(aiAnimNode->mRotationKeys[a].mValue);
				// rotation , framenumber
				if (newTake.m_KeyFrames.at(JointName).m_JointTransforms.find(timeStamp) != newTake.m_KeyFrames.at(JointName).m_JointTransforms.end())
				{
					newTake.m_KeyFrames.at(JointName).m_JointTransforms.find(timeStamp)->second.m_Rotation = rotation;
				}
				else
				{
					JointTransform newJointTransform;
					newJointTransform.m_Rotation = rotation;
					newTake.m_KeyFrames.at(JointName).m_JointTransforms[timeStamp] = newJointTransform;
				}
			}

			// Scales
			for (unsigned int a = 0; a < aiAnimNode->mNumScalingKeys; a++)
			{
				float timeStamp = aiAnimNode->mScalingKeys[a].mTime;
				glm::vec3 scale = AItoGLM(aiAnimNode->mScalingKeys[a].mValue);
				// scale , framenumber
				if (newTake.m_KeyFrames.at(JointName).m_JointTransforms.find(timeStamp) != newTake.m_KeyFrames.at(JointName).m_JointTransforms.end())
				{
					newTake.m_KeyFrames.at(JointName).m_JointTransforms.find(timeStamp)->second.m_Scale = scale;
				}
				else
				{
					JointTransform newJointTransform;
					newJointTransform.m_Scale = scale;
					newTake.m_KeyFrames.at(JointName).m_JointTransforms[timeStamp] = newJointTransform;
				}
			}
		}
		// add to takes
		takes.push_back(newTake);
	}
}

std::string GetJointName(const aiNodeAnim* animNode, const aiScene* scene)
{
	for (int i = 0; i < scene->mNumMeshes; i++)
	{	
		aiMesh* mesh = scene->mMeshes[i];
		if (mesh->HasBones())
		{
			for (int j = 0; j < mesh->mNumBones; j++)
			{
				aiBone* bone = mesh->mBones[j];
				std::string boneName{ bone->mName.C_Str() };
				std::string animNodeChannelName{ animNode->mNodeName.C_Str() };
				if (animNodeChannelName.find(boneName) != std::string::npos)
				{
					return boneName;
				}

			}
		}
	}
	return std::string();
}


std::vector<Take> AnimationLoader::LoadAnimations(std::string sourcePath)
{
	std::vector<Take> newTakes;
	AssimpLoadAnimation(sourcePath, newTakes);
	return newTakes;
}
