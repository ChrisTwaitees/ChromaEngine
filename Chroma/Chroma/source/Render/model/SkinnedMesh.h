#ifndef _CHROMA_SKINNED_MESH_
#define _CHROMA_SKINNED_MESH_

#include <vector>

#include <model/SkinnedVertex.h>
#include <animation/Skeleton.h>

class SkinnedMesh
{
	std::vector<ChromaSkinnedVertex> m_vertices;

	Skeleton m_skeleton;
public:
	SkinnedMesh();
	~SkinnedMesh();
};

#endif