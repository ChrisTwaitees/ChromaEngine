#ifndef _CHROMA_JOINT_
#define _CHROMA_JOINT_

// stl
#include <string>
#include <vector>

// thirdparty
#include <glm/glm.hpp>

// chroma

class Joint
{
	// Joint Data
	unsigned int m_ID;
	std::string m_Name;
	glm::mat4 m_OffsetMatrix;
	std::vector<std::pair<unsigned int, float>> m_VertexWeights;

public:
	// Accessors
	void SetID(unsigned int JointID) { m_ID = JointID; };
	void SetName(std::string Name) { m_Name = Name; };
	void SetOffsetMatrix(glm::mat4 newOffsetMatrix) { m_OffsetMatrix = newOffsetMatrix; };
	void AddVertexWeight(std::pair<unsigned int, float> newVertexWeight) { m_VertexWeights.push_back(newVertexWeight); };

	Joint();
	~Joint();
};

#endif