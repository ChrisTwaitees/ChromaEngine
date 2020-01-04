#ifndef _CHROMA_ANIMATION_
#define _CHROMA_ANIMATION_

// stl
#include <vector>
#include <iostream>
#include <map>

// thirdparty
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/quaternion.hpp>

// chroma
#include <common/CoreCommon.h>
#include <math/Math.h>


struct JointTransform
{
	glm::vec3 m_Translation{ 0.0f };
	glm::quat m_Rotation{ glm::quat() };
	glm::vec3 m_Scale{ 1.0f };
};


struct KeyFrame
{
	std::string m_JointName;
	// format is <timeStamp, JointTransform>
	std::map<float, JointTransform> m_JointTransforms;
};


struct Take
{
	std::string m_Name;
	int m_NumFrames{ 0.0f };
	float m_FPS{ 0.0f };
	float m_Duration{ 0.0f };
	std::map<std::string, KeyFrame> m_KeyFrames;
};

void AssimpLoadAnimation(std::string const& sourcePath, std::vector<Take>& takes);
void ProcessTakes(const aiScene* scene, aiNode* rootNode, std::vector<Take>& takes);
void RotateByJointOrient(aiQuaternion& rotation, const std::string& jointName, const aiScene*& scene);

std::string GetJointName(const aiNodeAnim* animNode, const aiScene* scene);

namespace AnimationLoader
{
	std::vector<Take> LoadAnimations(std::string sourcePath);
}


#endif