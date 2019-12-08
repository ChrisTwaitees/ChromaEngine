#ifndef _CHROMA_SKINNED_MESH_
#define _CHROMA_SKINNED_MESH_

// stl
#include <vector>
#include <string>

// chroma
#include <shader/Shader.h>
#include <texture/Texture.h>

#include <camera/Camera.h>
#include <light/Light.h>

#include <model/StaticMesh.h>
#include <model/SkinnedVertex.h>
#include <animation/Skeleton.h>

class SkinnedMesh : public StaticMesh
{
	// skeleton
	Skeleton m_skeleton;

	// mesh data

	std::vector<ChromaSkinnedVertex> m_vertices;


public:

	SkinnedMesh();
	~SkinnedMesh();
};

#endif