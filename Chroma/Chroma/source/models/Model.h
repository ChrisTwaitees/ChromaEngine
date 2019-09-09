#ifndef _MODEL_H_
#define _MODEL_H_
#include "./Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class Model
{
public:
	/*  Fuctions  */
	Model(std::string path) { loadModel(path); };
	void Draw(Shader &shader);
	~Model();
private:
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
};

#endif