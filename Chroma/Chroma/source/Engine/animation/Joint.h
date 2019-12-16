#ifndef _CHROMA_JOINT_
#define _CHROMA_JOINT_

// stl
#include <string>
#include <vector>
#include <iostream>

// thirdparty
#include <glm/glm.hpp>

// chroma

class Joint
{
	// Joint Data
	int m_ID;
	std::string m_Name;
	glm::mat4 m_LocalTransform;


public:
	// Accessors
	void SetID(int JointID) { m_ID = JointID; };
	void SetName(std::string Name) { m_Name = Name; };
	void SetLocalTransform(glm::mat4 newOffsetMatrix) { m_LocalTransform = newOffsetMatrix; };

	int GetID() const { return m_ID; };
	std::string GetName() const { return m_Name; };
	glm::mat4 GetLocalTransform() const { return m_LocalTransform; };

	Joint();
	~Joint();
};

#endif