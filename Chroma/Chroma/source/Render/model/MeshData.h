#ifndef _CHROMA_MESH_DATA_H_
#define _CHROMA_MESH_DATA_H_

#include <texture/Texture.h>
#include <animation/Skeleton.h>
#include <model/SkinnedVertex.h>
#include <model/Vertex.h>

struct MeshData
{
	std::string sourceDirectory{ "" };
	std::string sourcePath{ "" };
	bool isSkinned{ false };
	std::vector<ChromaSkinnedVertex> skinnedVerts;
	std::vector<ChromaVertex> verts;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Skeleton skeleton;
	bool isInitialized{ false };
	unsigned int VAO{ 0 }, VBO{ 0 }, EBO{ 0 };
};

#endif