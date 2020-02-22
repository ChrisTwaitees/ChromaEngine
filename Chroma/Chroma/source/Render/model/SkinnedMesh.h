#ifndef _CHROMA_SKINNED_MESH_
#define _CHROMA_SKINNED_MESH_

//common
#include <common/PrecompiledHeader.h>

// chroma
#include <model/StaticMesh.h>


class SkinnedMesh : public StaticMesh
{
public:
	glm::mat4 GetWorldTransform() override;

	// Accessors
	std::vector<ChromaSkinnedVertex> GetSkinnedVertices() { return m_SkinnedVertices; };
	virtual std::pair<glm::vec3, glm::vec3> GetBBox() override;
	Skeleton* GetSkeleton() { return &m_Skeleton; };

	// Functions
	void SetJointUniforms(Shader& skinnedShader) override;

	void Destroy() override;

	SkinnedMesh(std::vector<ChromaSkinnedVertex>& vertices_val, std::vector<unsigned int>& indices_val, std::vector<Texture>& textures_val, Skeleton& skeleton_val, glm::mat4 rootTransform_val);
	SkinnedMesh(MeshData const& newMeshData);
	SkinnedMesh(std::string const& sourcePath);
	SkinnedMesh() { m_IsRenderable = true; };
	~SkinnedMesh();
	
private:
	// skeleton
	Skeleton m_Skeleton;

	// root transforms
	glm::mat4 m_RootTransform{ 1 };
	glm::mat4 m_RootTransformInversed{ 1 };

	// mesh data
	std::vector<ChromaSkinnedVertex> m_SkinnedVertices;

	// functions
	void CalculateBBox() override;
	void SetupMesh() override;

};

#endif