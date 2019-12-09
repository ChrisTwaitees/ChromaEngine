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
	std::vector<ChromaSkinnedVertex> m_skinnedVertices;

	// functions
	void CalculateBBox() override;
	void CalculateCentroid() override;
	void setupMesh() override;

public:
	// Getters/Setters
	std::vector<ChromaSkinnedVertex> getSkinnedVertices() { return m_skinnedVertices; };
	virtual std::pair<glm::vec3, glm::vec3> GetBBox() override;

	// Functions

	SkinnedMesh(std::vector<ChromaSkinnedVertex> vertices_val, std::vector<unsigned int> indices_val, std::vector<Texture> textures_val, Skeleton skeleton_val);
	SkinnedMesh() { m_IsRenderable = true; };
	~SkinnedMesh();
};

#endif