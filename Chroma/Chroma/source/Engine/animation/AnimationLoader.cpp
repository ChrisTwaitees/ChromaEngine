#include "AnimationLoader.h"


void AssimpLoadAnimation(std::string const& sourcePath, std::vector<Take>& takes)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(sourcePath, NULL);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		CHROMA_WARN("ERROR::ASSIMP::{0}", importer.GetErrorString() );
		return;
	}
	else if (!scene->HasAnimations())
	{
		CHROMA_WARN("ERROR::ASSIMP::{0}", importer.GetErrorString());
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
		// Loading animation
		CHROMA_TRACE("-----------------------------------------");
		CHROMA_TRACE("ANIMATION LOADER:: Loading New Animation");
		// storing each animation as a "take"
		Take newTake;
		aiAnimation* aiAnim = scene->mAnimations[i];

		// AnimTake data
		newTake.m_Name = aiAnim->mName.C_Str();
		newTake.m_NumFrames = (int)aiAnim->mDuration;
		newTake.m_FPS = (float)aiAnim->mTicksPerSecond;
		newTake.m_Duration = 1.0f / newTake.m_FPS * (float)newTake.m_NumFrames;

		// Debug
		CHROMA_TRACE("ANIMATION LOADER:: Animation Clip Name : {0}", newTake.m_Name);
		CHROMA_TRACE("ANIMATION LOADER:: Animation Clip Number Frames : {0}", newTake.m_NumFrames);
		CHROMA_TRACE("ANIMATION LOADER:: Animation Clip FPS : {0}", newTake.m_FPS);
		CHROMA_TRACE("ANIMATION LOADER:: Animation Clip Duration : {0}", newTake.m_Duration);
		CHROMA_TRACE("-----------------------------------------");

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
				CHROMA_TRACE("ANIMATION LOADER:: Could not find correlating joint for channel : {0}", aiAnimNode->mNodeName.C_Str());
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
				float timeStamp = (float)aiAnimNode->mPositionKeys[a].mTime;
				glm::vec3 translation = AIToGLM(aiAnimNode->mPositionKeys[a].mValue);
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
				float timeStamp = (float)aiAnimNode->mRotationKeys[a].mTime;
				//aiQuaternion aiRotation = aiAnimNode->mRotationKeys[a].mValue;
				//RotateByJointOrient(aiRotation, JointName, scene);
				glm::quat rotation = AIToGLM(aiAnimNode->mRotationKeys[a].mValue);
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
				float timeStamp = (float)aiAnimNode->mScalingKeys[a].mTime;
				glm::vec3 scale = AIToGLM(aiAnimNode->mScalingKeys[a].mValue);
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

void RotateByJointOrient(aiQuaternion& rotation, const std::string& jointName, const aiScene*& scene)
{
	aiNode* joint = scene->mRootNode->FindNode((aiString)jointName);

	if (joint != NULL)
	{
		aiQuaternion boneOrient;
		aiVector3D test; 
		aiVector3D test1;
		((aiBone*)joint)->mOffsetMatrix.DecomposeNoScaling(boneOrient, test1);

		glm::vec3 euler = glm::degrees(glm::eulerAngles(AIToGLM(boneOrient)));
		rotation = boneOrient * rotation;
	}
	else
		return;
}

std::string GetJointName(const aiNodeAnim* animNode, const aiScene* scene)
{
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{	
		aiMesh* mesh = scene->mMeshes[i];
		if (mesh->HasBones())
		{
			for (unsigned int j = 0; j < mesh->mNumBones; j++)
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
