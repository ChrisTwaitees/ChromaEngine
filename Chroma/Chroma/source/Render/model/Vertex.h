#ifndef _CHROMA_VERTEX_
#define _CHROMA_VERTEX_

//common
#include <common/PrecompiledHeader.h>

struct ChromaVertex
{

	glm::vec3 m_position{ 0.0 };
	glm::vec3 m_normal{ 0.0 };
	glm::vec3 m_tangent{ 0.0 };
	glm::vec3 m_bitangent{ 0.0 };
	glm::vec2 m_texCoords{ 0.0 };
};


#endif