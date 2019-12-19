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
#include <math/Math.h>


struct JointTransform
{
	glm::vec3 m_Translation{ 0 };
	glm::quat m_Rotation{ glm::quat() };
	glm::vec3 m_Scale{ 1 };
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
	int m_NumFrames;
	float m_FPS;
	float m_Duration;
	std::vector<KeyFrame> m_KeyFrames;
};

void AssimpLoadAnimation(std::string const& sourcePath, std::vector<Take>& takes);
void ProcessTakes(const aiScene* scene, aiNode* rootNode, std::vector<Take>& takes);


std::string GetJointName(const aiNodeAnim* animNode, const aiScene* scene);

namespace AnimationLoader
{
	std::vector<Take> LoadAnimations(std::string sourcePath);
}


#endif