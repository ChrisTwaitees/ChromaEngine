#ifndef _MODEL_H_
#define _MODEL_H_

#include <model/StaticMesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public ChromaMeshComponent
{
	// calculate attrs
	virtual void calcBBox();
	virtual void calcCentroid();

	// Model Data
	std::vector<ChromaVertex> m_vertices;
	std::vector<Texture> m_textures;
	std::vector<StaticMesh*> m_meshes;
	std::string m_directory;

	// Functions
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	StaticMesh* processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		Texture::TYPE typeName);
public:
	// Draw
	void Draw(Shader& shader) override;
	void Draw(Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix) override;
	void Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix) override;
	void DrawUpdateMaterials(Shader& shader) override;
	void DrawUpdateTransforms(Camera& renderCam, glm::mat4& modelMatrix) override;

	// Getters/Settters
	int getNumTextures() override { return m_textures.size(); };
	Shader* getShader() override { return m_meshes[0]->getShader(); };
	void bindShader(Shader* const& newShader) override;
	glm::mat4 getTransformationMatrix() override { return m_transformationMatrix; };
	virtual std::vector<ChromaVertex> getVertices() { return m_vertices; };
	virtual std::pair<glm::vec3, glm::vec3> getBBox();
	virtual glm::vec3 getCentroid();

	// Component requirement 
	void bindTextures(std::vector<Texture> textures_val) override {};
	void bindTexture(Texture texture_val) override;

	// Shader Uniforms
	void setMat4(std::string name, glm::mat4 value) override;
	void setInt(std::string name, int value) override;
	void setFloat(std::string name, float value) override;

	// Constructors
	Model(const Model& ref) 
	{
		std::cout << "Copy Constructor of Model Invoked" << std::endl;
	}
	Model(std::string path) { loadModel(path); };
	Model() {};
	~Model();
};

#endif