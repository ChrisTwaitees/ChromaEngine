#ifndef _MODEL_H_
#define _MODEL_H_

// thirdparty
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// chroma
#include <math/ChromaMath.h>
#include <model/StaticMesh.h>
#include <model/SkinnedMesh.h>

class Model : public ChromaMeshComponent
{
	// calculate attrs
	virtual void calcBBox();
	virtual void calcCentroid();

	// Model Data
	bool m_isSkinned{ false };
	std::vector<ChromaVertex> m_vertices;
	std::vector<ChromaSkinnedVertex> m_skinnedVertices;
	
	std::vector<Texture> m_textures;
	std::vector<ChromaMeshComponent*> m_meshes;
	std::string m_directory;

	// Functions
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	ChromaMeshComponent* processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		Texture::TYPE typeName);
public:
	// Draw
	void Draw(Shader& shader) override;
	void Draw(Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix) override;
	void Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix) override;
	void DrawUpdateMaterials(Shader& shader) override;
	void DrawUpdateTransforms(Camera& renderCam, glm::mat4& modelMatrix) override;

	// Getters
	glm::mat4 getTransformationMatrix() override { return m_transformationMatrix; };
	// render components
	Shader* getShader() override { return m_meshes[0]->getShader(); };
	void bindShader(Shader* const& newShader) override;
	int getNumTextures() override { return m_textures.size(); };
	// verts
	std::vector<ChromaVertex> getVertices() override { return m_vertices; };
	std::vector<ChromaSkinnedVertex> getSkinnedVertices() const { return  m_skinnedVertices; };
	// bbox
	std::pair<glm::vec3, glm::vec3> getBBox();
	glm::vec3 getCentroid();
	// Setters

	// Component requirement 
	void bindTextures(std::vector<Texture> textures_val) override {};
	void bindTexture(Texture texture_val) override;

	// Shader Uniforms
	void setMat4(std::string name, glm::mat4 value) override;
	void setInt(std::string name, int value) override;
	void setFloat(std::string name, float value) override;

	// Constructors
	Model(std::string path) { loadModel(path); };
	~Model();
};

#endif