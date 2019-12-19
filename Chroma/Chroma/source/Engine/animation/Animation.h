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

struct KeyFrame
{
	std::string m_JointName;
	unsigned int m_JointID;
	// format is <data, framenumber>
	std::vector<std::pair<glm::vec3, float>> m_Positions;
	std::vector<std::pair<glm::quat, float>> m_Rotations;
	std::vector<std::pair<glm::vec3, float>> m_Scales;
};


struct AnimTake
{
	std::string m_Name;
	glm::mat4 m_RootTransform;
	double m_NumFrames;
	double m_FPS;
	double m_Duration;
	std::vector<KeyFrame> m_KeyFrames;
};


class Animation
{
	std::vector<AnimTake> m_AnimTakes;

	void LoadAnimation(std::string const& sourcePath);
	void ProcessAnimation(const aiScene* scene, aiNode* rootNode);

public:

	std::vector<AnimTake> GetAnimTakes() const { return m_AnimTakes; };

	Animation(std::string sourcePath);
	~Animation();
};

#endif