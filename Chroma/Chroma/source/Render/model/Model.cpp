#include "Model.h"
#include "../texture/stb_image.h"

void Model::Draw(Shader &shader)
{
	for (StaticMesh* mesh : meshes)
		mesh->Draw(shader);
}

void Model::Draw(Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights, glm::mat4& transformMatrix)
{
	if (mShader)
		Draw(*mShader, RenderCamera, Lights, transformMatrix);
	else
		for (StaticMesh* mesh : meshes)
				mesh->Draw(RenderCamera, Lights, transformMatrix);
}

void Model::Draw(Shader& shader, Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights, glm::mat4& transformMatrix)
{
	for (StaticMesh* mesh : meshes)
		mesh->Draw(shader, RenderCamera, Lights, transformMatrix);
}

void Model::DrawUpdateMaterials(Shader& shader)
{
	for (StaticMesh* mesh : meshes)
		mesh->DrawUpdateMaterials(shader);
}

void Model::DrawUpdateTransforms(Camera& renderCam, glm::mat4& modelMatrix)
{
	for (StaticMesh* mesh : meshes)
		mesh->DrawUpdateTransforms(renderCam, modelMatrix);
}


void Model::bindTexture(Texture texture_val)
{
	for (StaticMesh* mesh : meshes)
		mesh->bindTexture(texture_val);
}

void Model::setMat4(std::string name, glm::mat4 value)
{
	for (StaticMesh* mesh : meshes)
		mesh->setMat4(name, value);
}

void Model::setInt(std::string name, int value)
{
	for (StaticMesh* mesh : meshes)
		mesh->setInt(name, value);
}

void Model::setFloat(std::string name, float value)
{
	for (StaticMesh* mesh : meshes)
		mesh->setFloat(name, value);
}

Model::~Model()
{
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

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

StaticMesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<unsigned int> indices;
	std::vector <Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		ChromaVertex vertex;
		// process vertex
		// positions
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		vertex.setPosition(position);

		// normals
		glm::vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		vertex.setNormal(normal);

		// tangents
		glm::vec3 tangent;
		tangent.x = mesh->mTangents[i].x;
		tangent.y = mesh->mTangents[i].y;
		tangent.z = mesh->mTangents[i].z;
		vertex.setTangent(tangent);

		// bitangents 
		glm::vec3 bitangent;
		bitangent.x = mesh->mBitangents[i].x;
		bitangent.y = mesh->mBitangents[i].y;
		bitangent.z = mesh->mBitangents[i].z;
		vertex.setBitangent(bitangent);

		// texture coords
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 UV1;
			UV1.x = mesh->mTextureCoords[0][i].x;
			UV1.y = mesh->mTextureCoords[0][i].y;
			vertex.setTexCoords(UV1);
		}
		else
			vertex.setTexCoords(glm::vec2(0.0f, 0.0f));

		// add new vertex
		m_vertices.push_back(vertex);
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
		// diffuse textures
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, Texture::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// specular textures
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, Texture::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// normal textures
		std::vector<Texture> normalMaps = loadMaterialTextures(material,
			aiTextureType_HEIGHT, Texture::NORMAL);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	return new StaticMesh(m_vertices, indices, textures);
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