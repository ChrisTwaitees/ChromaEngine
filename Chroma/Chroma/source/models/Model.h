#ifndef _MODEL_H_
#define _MODEL_H_
#include "../shaders/Shader.h"
#include "./Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
	/*  Fuctions  */
	Model(char* path) { loadModel(path); };
	void Draw(Shader shader);
	~Model();
private:
	/*  Model Data  */
	std::vector<MeshTexture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	/*  Functions  */
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		MeshTexture::TYPE typeName);
};

#endif