#include "Model.h"
#include <model/StaticMesh.h>
#include <model/SkinnedMesh.h>
#include <scene/Scene.h>


// DRAW
void Model::Draw(Shader& shader)
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->Draw(shader);
}

void Model::Draw(Camera& RenderCamera)
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->Draw(RenderCamera);
}

void Model::Draw(Shader& shader, Camera& RenderCamera)
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->Draw(shader, RenderCamera);
}

void Model::DrawUpdateMaterials(Shader& shader)
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->DrawUpdateMaterials(shader);
}

void Model::DrawUpdateTransforms(Camera& renderCam)
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->DrawUpdateTransforms(renderCam);
}

Shader& Model::GetShader()
{
	for (UID const& uid : m_MeshUIDs)
		return static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->GetShader();
}

void Model::SetShader(Shader& shader)
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->SetShader(shader);
}

std::vector<ChromaVertex> Model::GetVertices()
{
	std::vector<ChromaVertex> verts;
	for (UID const& meshUID : m_MeshUIDs)
	{
		MeshComponent* meshComp = static_cast<MeshComponent*>(Chroma::Scene::GetComponent(meshUID));

		for (ChromaVertex const& vert : meshComp->GetVertices())
		{
			verts.push_back(vert);
		}
	}

	return verts;

}

// BBOX
std::pair<glm::vec3, glm::vec3> Model::GetBBox()
{
	CalculateBBox();
	return std::make_pair(m_BBoxMin, m_BBoxMax);
}

glm::vec3 Model::GetCentroid()
{
	CalculateCentroid();
	return m_Centroid;
}

void Model::SetIsLit(bool const& check)
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->SetIsLit(check);
}

void Model::SetIsForwardLit(bool const& check)
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->SetIsForwardLit(check);
}


// TEXTURES
void Model::AddTexture(Texture& texture_val)
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->AddTexture(texture_val);
}

void Model::SetMaterial(const Material& newMaterial)
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->SetMaterial(newMaterial);
}

void Model::SetTextureSet(std::vector<Texture>& textures_val)
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->SetTextureSet(textures_val);
}

// UNIFORMS
void Model::SetMat4(std::string name, glm::mat4 value)
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->SetMat4(name, value);
}

void Model::SetInt(std::string name, int value)
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->SetInt(name, value);
}

void Model::SetFloat(std::string name, float value)
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->SetFloat(name, value);
}

void Model::SetJointUniforms(Shader& skinnedShader)
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->SetJointUniforms(skinnedShader);
}

void Model::CalculateBBox()
{
	// collecting all bboxes within mesh components of entity and returning overall
	std::vector<std::pair<glm::vec3, glm::vec3>> bboxes;
	for (UID const& uid : m_MeshUIDs)
		bboxes.push_back(((MeshComponent*)Chroma::Scene::GetComponent(uid))->GetBBox());

	// once collected, calculate new min and max bbox
	glm::vec3 newMinBBox(99999.00, 99999.00, 99999.00);
	glm::vec3 newMaxBBox(0.0, 0.0, 0.0);
	for (std::pair<glm::vec3, glm::vec3> MinMaxBBoxes : bboxes)
	{
		newMinBBox = glm::min(newMinBBox, MinMaxBBoxes.first);
		newMaxBBox = glm::max(newMaxBBox, MinMaxBBoxes.second);
	}
	// re-establishing min and max bboxes
	m_BBoxMin = newMinBBox;
	m_BBoxMax = newMaxBBox;
}

void Model::CalculateCentroid()
{
	m_Centroid = m_BBoxMin + ((m_BBoxMin - m_BBoxMax) * glm::vec3(0.5));
}


// LOADING
void Model::LoadModel(std::string path)
{
	m_SourcePath = path;

	for (MeshData& meshData : Chroma::ResourceManager::LoadModels(path))
	{
		if (meshData.isSkinned)
		{
			m_IsSkinned = true;
			SkinnedMesh* newSkinnedMesh = new SkinnedMesh(meshData);
			newSkinnedMesh->SetSourcePath(path);
			Chroma::Scene::AddMeshComponent(newSkinnedMesh);
			m_MeshUIDs.push_back(newSkinnedMesh->GetUID());
		}
		else
		{
			StaticMesh* newStaticMesh = new StaticMesh(meshData);
			newStaticMesh->SetSourcePath(path);
			Chroma::Scene::AddMeshComponent(newStaticMesh);
			m_MeshUIDs.push_back(newStaticMesh->GetUID());
		}
	}

}

void Model::Init()
{
	// set entity UID to all meshes under model
	for (UID const& uid : m_MeshUIDs)
	{
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->SetParentEntityUID(GetParentEntityUID());
	}

	// set component m_Type
	m_Type = Chroma::Type::Component::kModelComponent;

	CMPNT_INITIALIZED
}

void Model::RebuildMesh()
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->RebuildMesh();
}


void Model::Destroy()
{
	// Material
	m_Material.Destroy();

	// verts
	m_IsSkinned ? m_SkinnedVertices.clear() : m_vertices.clear();

	CMPNT_DESTROYED
}

void Model::CleanUp()
{
	for (UID const& uid : m_MeshUIDs)
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->CleanUp();
}

void Model::Serialize(ISerializer*& serializer)
{
	CMPNT_SERIALIZE_BEGIN

	// Properties
	// Transform
	serializer->AddProperty("m_Translation", &m_Translation);
	serializer->AddProperty("m_Rotation", &m_Rotation);
	serializer->AddProperty("m_Scale", &m_Scale);

	// File Properties
	serializer->AddProperty("m_SourcePath", &m_SourcePath);

	// MeshUIDs
	serializer->AddProperty("m_MeshUIDs", m_MeshUIDs);
}


Model::~Model()
{
}