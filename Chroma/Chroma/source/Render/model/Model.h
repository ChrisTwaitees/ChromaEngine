#ifndef _MODEL_H_
#define _MODEL_H_

#include "entity/ChromaEntity.h"
#include "model/StaticMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public ChromaMeshComponent
{
protected:
	// Transformations


	// Model Data
	std::vector<ChromaVertex> m_vertices;
	std::vector<Texture> textures_loaded;
	std::vector<StaticMesh*> meshes;
	std::string directory;
	Shader* mShader = nullptr;

	// Functions
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	StaticMesh* processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		Texture::TYPE typeName);
public:
	// Draw
	void Draw(Shader& shader) override;
	void Draw(Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights, glm::mat4& transformMatrix) override;
	void Draw(Shader& shader, Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights, glm::mat4& transformMatrix) override;
	void DrawUpdateMaterials(Shader& shader) override;
	void DrawUpdateTransforms(Camera& renderCam, glm::mat4& modelMatrix) override;

	// Getters/Settters
	int getNumTextures() override { return textures_loaded.size(); };
	void bindShader(Shader* newShader) override { mShader = newShader; };
	Shader* getShader() override { return mShader; };
	glm::mat4 getTransformationMatrix() override { return TransformationMatrix; };
	virtual std::vector<ChromaVertex> getVertices() { return m_vertices; };
	virtual std::pair<glm::vec3, glm::vec3> getBBox() {};

	// Component requirement 
	void bindTextures(std::vector<Texture> textures_val) override {};
	void bindTexture(Texture texture_val) override;

	// Shader Uniforms
	void setMat4(std::string name, glm::mat4 value) override;
	void setInt(std::string name, int value) override;
	void setFloat(std::string name, float value) override;

	Model(std::string path) { loadModel(path); };
	Model() {};
	~Model();
};

#endif