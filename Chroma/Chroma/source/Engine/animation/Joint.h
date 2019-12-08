#ifndef _CHROMA_JOINT_
#define _CHROMA_JOINT_

#include <glm/glm.hpp>

class Joint
{
	int m_id;
	glm::mat4 m_jointTransform;

public:
	Joint();
	~Joint();
};

#endif