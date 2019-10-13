#include "Model.h"
#include "../texture/stb_image.h"

void Model::Draw(Shader &shader)
{
	for (StaticMesh mesh : meshes)
		mesh.Draw(shader);
}

void Model::Draw(Camera& RenderCamera, std::vector<Light*>& Lights, glm::mat4& transformMatrix)
{
	for (StaticMesh mesh : meshes)
		mesh.Draw(RenderCamera, Lights, transformMatrix);
}

void Model::Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*>& Lights, glm::mat4& transformMatrix)
{
	for (StaticMesh mesh : meshes)
		mesh.Draw(shader, RenderCamera, Lights, transformMatrix);
}

Model::~Model()
{
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process node's meshes (if it has any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// check if node has children, if so recursively search for meshes
	for (unsigned int i = 0; i< node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

StaticMesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector <Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex
		// positions
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		vertex.Position = position;

		// normals
		glm::vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		vertex.Normal = normal;

		// texture coords
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 UV1;
			UV1.x = mesh->mTextureCoords[0][i].x;
			UV1.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = UV1;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	// process indices
	// primitive (face) indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, Texture::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, Texture::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return StaticMesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, Texture::TYPE typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip{ false };
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.loadFromFile(str.C_Str(), directory);
			texture.type = typeName;
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}