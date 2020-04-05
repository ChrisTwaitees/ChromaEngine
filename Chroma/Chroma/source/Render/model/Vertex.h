#ifndef _CHROMA_VERTEX_
#define _CHROMA_VERTEX_

//common
#include <common/PrecompiledHeader.h>

struct ChromaVertex
{
	// position
	glm::vec3 m_position{ 0.0 };
	// normal
	glm::vec3 m_normal{ 0.0 };

	// tagent
	glm::vec3 m_tangent{ 0.0 };
	// bitangent
	glm::vec3 m_bitangent{ 0.0 };
	// uvs
	glm::vec2 m_texCoords{ 0.0 };
	// colors
	glm::vec4 m_color{ 0.0 };
};


#endif