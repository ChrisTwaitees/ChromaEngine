#ifndef _CHROMA_MESH_DATA_H_
#define _CHROMA_MESH_DATA_H_

#include <texture/Texture.h>
#include <animation/Skeleton.h>
#include <model/SkinnedVertex.h>
#include <model/Vertex.h>

struct MeshData
{
	// File Source Data
	std::string sourceDirectory{ "" };
	std::string sourcePath{ "" };

	// Vertices
	std::vector<ChromaVertex> verts;
	std::vector<ChromaSkinnedVertex> skinnedVerts;
	std::vector<unsigned int> indices;

	// Animation
	bool isSkinned{ false };

	// Loaded
	bool isLoaded{ false };
	bool isRenderBuffersInitialized{ false };

	// Dimensions	
	glm::vec3 centroid;
	glm::vec3 bboxMin;
	glm::vec3 bboxMax;

	// Animation
	Skeleton skeleton;

	// Textures
	std::vector<Texture> textures;

	// Render Buffers
	unsigned int VAO{ 0 }, VBO{ 0 }, EBO{ 0 };
};

#endif