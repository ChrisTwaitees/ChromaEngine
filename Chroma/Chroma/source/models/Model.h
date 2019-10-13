#ifndef _MODEL_H_
#define _MODEL_H_

#include "../entity/ChromaEntity.h"
#include "../models/StaticMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public StaticMesh
{
protected:
	/* Transforms */
	glm::mat4 transformMatrix = glm::mat4(1);
	/*  Model Data  */
	std::vector<Texture> textures_loaded;
	std::vector<StaticMesh> meshes;
	std::string directory;
	/*  Functions  */
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	StaticMesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		Texture::TYPE typeName);
public:
	/*  Functions  */
	void Draw(Shader& shader) override;
	void Draw(Camera& RenderCamera, std::vector<Light*>& Lights, glm::mat4& transformMatrix) override;
	void Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*>& Lights, glm::mat4& transformMatrix) override;

	/* Getters Setters */
	int getNumTextures() { return textures_loaded.size(); };

	/* Constructors*/
	Model(std::string path) { loadModel(path); };
	Model() {};
	~Model();
};

#endif