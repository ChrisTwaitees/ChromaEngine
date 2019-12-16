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
	Skeleton m_Skeleton;

	// root transforms
	glm::mat4 m_RootTransform{ 1 };
	glm::mat4 m_RootTransformInversed{ 1 };

	// mesh data
	std::vector<ChromaSkinnedVertex> m_skinnedVertices;

	// functions
	void CalculateBBox() override;
	void setupMesh() override;

public:
	// Getters/Setters
	std::vector<ChromaSkinnedVertex> getSkinnedVertices() { return m_skinnedVertices; };
	virtual std::pair<glm::vec3, glm::vec3> GetBBox() override;

	// Functions
	void SetJointUniforms(Shader& skinnedShader) override;

	SkinnedMesh(std::vector<ChromaSkinnedVertex>& vertices_val, std::vector<unsigned int>& indices_val, std::vector<Texture>& textures_val, Skeleton& skeleton_val, glm::mat4 rootTransform_val);
	SkinnedMesh() { m_IsRenderable = true; };
	~SkinnedMesh();
};

#endif