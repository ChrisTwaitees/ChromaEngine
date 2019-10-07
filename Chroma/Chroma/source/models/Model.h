#ifndef _MODEL_H_
#define _MODEL_H_

#include "../entity/Entity.h"
#include "../models/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public Entity
{
protected:
	/* Transforms */
	glm::mat4 modelMat = glm::mat4(1);
	/*  Model Data  */
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	/*  Functions  */
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		Texture::TYPE typeName);
public:
	/*  Functions  */
	virtual void Draw(Shader& shader) override;
	virtual void Draw() override;

	/* Getters Setters */
	int getNumTextures() { return textures_loaded.size(); };

	/* Constructors*/
	Model(std::string path) { loadModel(path); };
	Model() {};
	~Model();
};

#endif