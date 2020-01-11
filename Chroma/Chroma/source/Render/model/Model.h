#ifndef _MODEL_H_
#define _MODEL_H_

// thirdparty
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// chroma
#include <math/Math.h>
#include <component/MeshComponent.h>
#include <content/ModelLoader.h>


class Model : public MeshComponent
{
	// calculate dimensions
	virtual void CalculateBBox() override;
	virtual void CalculateCentroid() override;

	// Model Data
	std::vector<UID> m_MeshUIDs;
	// verts
	std::vector<ChromaVertex> m_vertices;
	std::vector<ChromaSkinnedVertex> m_SkinnedVertices;
	//textures
	std::vector<Texture> m_Textures;
	// Functions
	void LoadModel(std::string path);

public:
	// Draw
	void Draw(Shader& shader) override;
	void Draw(Camera& RenderCamera) override;
	void Draw(Shader& shader, Camera& RenderCamera) override;
	void DrawUpdateMaterials(Shader& shader) override;
	void DrawUpdateTransforms(Camera& renderCam) override;

	// Accessors	
	std::pair<glm::vec3, glm::vec3> GetBBox() override;
	glm::vec3 GetCentroid();
	// render components
	Shader* GetShader();
	void SetShader(Shader* const& newShader) override;
	int GetNumTextures() override { return (int)m_Textures.size(); };
	// verts
	std::vector<ChromaVertex> GetVertices() override { return m_vertices; };
	std::vector<ChromaSkinnedVertex> GetSkinnedVertices() const { return  m_SkinnedVertices; };
	// animation/ skinning

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