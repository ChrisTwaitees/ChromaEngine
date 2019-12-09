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
	unsigned int m_ID;
	std::string m_name;
	glm::mat4 m_offsetMatrix;
	std::vector<std::pair<unsigned int, float>> m_vertexWeights;

public:
	void setID(unsigned int JointID) { m_ID = JointID; };
	void setName(std::string Name) { m_name = Name; };
	void setOffsetMatrix(glm::mat4 newOffsetMatrix) { m_offsetMatrix = newOffsetMatrix; };
	void addVertexWeight(std::pair<unsigned int, float> newVertexWeight) { m_vertexWeights.push_back(newVertexWeight); };

	Joint();
	~Joint();
};

#endif