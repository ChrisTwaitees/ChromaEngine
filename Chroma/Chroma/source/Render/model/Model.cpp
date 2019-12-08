#include "Model.h"
#include <texture/stb_image.h>

void Model::Draw(Shader &shader)
{
	for (ChromaMeshComponent*& mesh : m_meshes)
		mesh->Draw(shader);
}

void Model::Draw(Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix)
{
	for (ChromaMeshComponent*& mesh : m_meshes)
			mesh->Draw(RenderCamera, Lights, transformMatrix);
}

void Model::Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix)
{
	for (ChromaMeshComponent*& mesh : m_meshes)
		mesh->Draw(shader, RenderCamera, Lights, transformMatrix);
}

void Model::DrawUpdateMaterials(Shader& shader)
{
	for (ChromaMeshComponent*& mesh : m_meshes)
		mesh->DrawUpdateMaterials(shader);
}

void Model::DrawUpdateTransforms(Camera& renderCam, glm::mat4& modelMatrix)
{
	for (ChromaMeshComponent* mesh : m_meshes)
		mesh->DrawUpdateTransforms(renderCam, modelMatrix);
}

void Model::bindShader(Shader* const& newShader)
{
	for (ChromaMeshComponent* mesh : m_meshes)
		mesh->bindShader(newShader);
}


std::pair<glm::vec3, glm::vec3> Model::getBBox()
{
	calcBBox();
	return std::make_pair(m_bbox_min, m_bbox_max);
}

glm::vec3 Model::getCentroid()
{
	calcCentroid();
	return m_centroid;
}

void Model::bindTexture(Texture texture_val)
{
	for (ChromaMeshComponent* mesh : m_meshes)
		mesh->bindTexture(texture_val);
}

void Model::setMat4(std::string name, glm::mat4 value)
{
	for (ChromaMeshComponent* mesh : m_meshes)
		mesh->setMat4(name, value);
}

void Model::setInt(std::string name, int value)
{
	for (ChromaMeshComponent* mesh : m_meshes)
		mesh->setInt(name, value);
}

void Model::setFloat(std::string name, float value)
{
	for (ChromaMeshComponent* mesh : m_meshes)
		mesh->setFloat(name, value);
}


Model::~Model()
{
	for (ChromaMeshComponent* mesh : m_meshes)
		delete mesh;
}

void Model::calcBBox()
{
	// collecting all bboxes within mesh components of entity and returning overall
	std::vector<std::pair<glm::vec3, glm::vec3>> bboxes;
	for (ChromaMeshComponent*& mesh : m_meshes)
		bboxes.push_back(mesh->getBBox());

	// once collected, calculate new min and max bbox
	glm::vec3 newMinBBox(99999.00, 99999.00, 99999.00);
	glm::vec3 newMaxBBox(0.0, 0.0, 0.0);
	for (std::pair<glm::vec3, glm::vec3> MinMaxBBoxes : bboxes)
	{
		newMinBBox = glm::min(newMinBBox, MinMaxBBoxes.first);
		newMaxBBox = glm::max(newMaxBBox, MinMaxBBoxes.second);
	}
	// re-establishing min and max bboxes
	m_bbox_min = newMinBBox;
	m_bbox_max = newMaxBBox;
}

void Model::calcCentroid()
{
	m_centroid = m_bbox_min + ((m_bbox_min - m_bbox_max) * glm::vec3(0.5));
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
	m_directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process node's meshes (if it has any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene));
	}
	// check if node has children, if so recursively search for meshes
	for (unsigned int i = 0; i< node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

ChromaMeshComponent* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;

	// check if mesh is skinned
	m_isSkinned = mesh->HasBones();

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// process vertex
		// positions
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;

		// normals
		glm::vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;

		// tangents
		glm::vec3 tangent;
		tangent.x = mesh->mTangents[i].x;
		tangent.y = mesh->mTangents[i].y;
		tangent.z = mesh->mTangents[i].z;

		// bitangents 
		glm::vec3 bitangent;
		bitangent.x = mesh->mBitangents[i].x;
		bitangent.y = mesh->mBitangents[i].y;
		bitangent.z = mesh->mBitangents[i].z;

		// texture coords
		glm::vec2 UV1;
		if (mesh->mTextureCoords[0])
		{
			UV1.x = mesh->mTextureCoords[0][i].x;
			UV1.y = mesh->mTextureCoords[0][i].y;
		}
		else
			UV1 = glm::vec2(0.0f, 0.0f);

		// add new vertex
		if (m_isSkinned)
		{
			ChromaSkinnedVertex vertex;
			vertex.setPosition(position);
			vertex.setNormal(normal);
			vertex.setTangent(tangent);
			vertex.setBitangent(bitangent);
			vertex.setTexCoords(UV1);
			//mesh->mBones[]
			m_skinnedVertices.push_back(vertex);
		}
		else
		{
			ChromaVertex vertex;
			vertex.setPosition(position);
			vertex.setNormal(normal);
			vertex.setTangent(tangent);
			vertex.setBitangent(bitangent);
			vertex.setTexCoords(UV1);
			m_vertices.push_back(vertex);
		}

	}

	// process indices
	// primitive (face) indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			m_indices.push_back(face.mIndices[j]);
	}
	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// diffuse textures
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, Texture::ALBEDO);
		m_textures.insert(m_textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// specular textures
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, Texture::METALNESS);
		m_textures.insert(m_textures.end(), specularMaps.begin(), specularMaps.end());
		// normal textures
		std::vector<Texture> normalMaps = loadMaterialTextures(material,
			aiTextureType_HEIGHT, Texture::NORMAL);
		m_textures.insert(m_textures.end(), normalMaps.begin(), normalMaps.end());
	}

	if (m_isSkinned)
	{
		return new SkinnedMesh(m_skinnedVertices, m_indices, m_textures);
	}
	else
		return new StaticMesh(m_vertices, m_indices, m_textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, Texture::TYPE typeName)
{
	std::vector<Texture> m_textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip{ false };
		for (unsigned int j = 0; j < m_textures.size(); j++)
		{
			if (std::strcmp(m_textures[j].path.data(), str.C_Str()) == 0)
			{
				m_textures.push_back(m_textures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.loadFromFile(str.C_Str(), m_directory);
			texture.type = typeName;
			m_textures.push_back(texture);
			m_textures.push_back(texture);
		}
	}
	return m_textures;
}