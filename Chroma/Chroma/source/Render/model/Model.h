#ifndef _MODEL_H_
#define _MODEL_H_

// thirdparty
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// chroma
#include <math/Math.h>
#include <model/StaticMesh.h>
#include <model/SkinnedMesh.h>

class Model : public MeshComponent
{
	// calculate dimensions
	virtual void CalculateBBox();
	virtual void CalculateCentroid();

	// Model Data
	std::string m_directory;
	std::vector<MeshComponent*> m_meshes;
	// verts
	std::vector<ChromaVertex> m_vertices;
	std::vector<ChromaSkinnedVertex> m_SkinnedVertices;
	//textures
	std::vector<Texture> m_Textures;

	// Functions
	// meshes
	void LoadModel(std::string path);
	void GetChildMeshNodes(aiNode* node, const aiScene* scene);
	MeshComponent* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	// skinning and skeletons
	void SetVertSkinningData(ChromaSkinnedVertex& vert, std::pair<int, float>  const& jointIDWeight);
	void ProcessSkeleton(const aiScene* scene, const aiMesh* mesh, Skeleton& skeleton);
	void GetChildJointIDs(aiNode* node, Skeleton& skeleton, std::vector<int>& childJointIDs);
	void GetParentJointID(const aiNode* node, Skeleton& skeleton, int& parentJointID);
	void NormalizeSkinningWeights();
	// textures
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,Texture::TYPE typeName);

public:
	// Draw
	void Draw(Shader& shader) override;
	void Draw(Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix) override;
	void Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix) override;
	void DrawUpdateMaterials(Shader& shader) override;
	void DrawUpdateTransforms(Camera& renderCam, glm::mat4& modelMatrix) override;

	// Accessors	
	glm::mat4 GetTransform() override { return m_TransformationMatrix; };
	std::pair<glm::vec3, glm::vec3> GetBBox();
	glm::vec3 GetCentroid();
	// render components
	Shader* GetShader() override { return m_meshes[0]->GetShader(); };
	void SetShader(Shader* const& newShader) override;
	int GetNumTextures() override { return m_Textures.size(); };
	// verts
	std::vector<ChromaVertex> GetVertices() override { return m_vertices; };
	std::vector<ChromaSkinnedVertex> GetSkinnedVertices() const { return  m_SkinnedVertices; };
	// animation/ skinning
	Skeleton* GetSkeleton();
	void SetScale(float const& newScale);
	void SetTranslation(glm::vec3 const& newPosition);
	void SetRotation(glm::quat const& newRotation);

	// Component requirement 
	void SetTextures(std::vector<Texture> textures_val) override {};
	void AddTexture(Texture texture_val) override;

	// Shader Uniforms
	void SetMat4(std::string name, glm::mat4 value) override;
	void SetInt(std::string name, int value) override;
	void SetFloat(std::string name, float value) override;
	void SetJointUniforms(Shader& skinnedShader) override;

	// Constructors
	Model(std::string path) { LoadModel(path); };
	~Model();
};

#endif