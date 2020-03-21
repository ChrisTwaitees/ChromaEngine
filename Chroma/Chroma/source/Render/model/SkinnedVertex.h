#ifndef _CHROMA_SKINNED_VERTEX_
#define _CHROMA_SKINNED_VERTEX_

//common
#include <common/PrecompiledHeader.h>
#include <model/Vertex.h>

struct ChromaSkinnedVertex : public ChromaVertex
{
	glm::ivec4 m_jointIDs { 0 };
	glm::vec4 m_jointWeights{ 0.0 };
};


#endif