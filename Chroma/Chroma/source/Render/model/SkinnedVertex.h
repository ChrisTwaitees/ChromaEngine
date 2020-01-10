#ifndef _CHROMA_SKINNED_VERTEX_
#define _CHROMA_SKINNED_VERTEX_

#include <glm/glm.hpp>
#include <vector>

struct ChromaSkinnedVertex
{
	glm::vec3 m_position{ 0.0 };
	glm::vec3 m_normal{ 0.0 };
	glm::vec3 m_tangent{ 0.0 };
	glm::vec3 m_bitangent{ 0.0 };
	glm::vec2 m_texCoords{ 0.0 };
	glm::ivec4 m_jointIDs { 0 };
	glm::vec4 m_jointWeights{ 0.0 };
};


#endif