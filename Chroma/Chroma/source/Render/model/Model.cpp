#include "Model.h"
#include <model/StaticMesh.h>
#include <model/SkinnedMesh.h>
#include <scene/Scene.h>


// DRAW
void Model::Draw(Shader &shader)
{
	for (UID const& uid : m_MeshUIDs)
		((MeshComponent*)Chroma::Scene::GetComponent(uid))->Draw(shader);
}

void Model::Draw(Camera& RenderCamera)
{
	for (UID const& uid : m_MeshUIDs)
		((MeshComponent*)Chroma::Scene::GetComponent(uid))->Draw(RenderCamera);
}

void Model::Draw(Shader& shader, Camera& RenderCamera)
{
	for (UID const& uid : m_MeshUIDs)
		((MeshComponent*)Chroma::Scene::GetComponent(uid))->Draw(shader, RenderCamera);
}

void Model::DrawUpdateMaterials(Shader& shader)
{
	for (UID const& uid : m_MeshUIDs)
		((MeshComponent*)Chroma::Scene::GetComponent(uid))->DrawUpdateMaterials(shader);
}

void Model::DrawUpdateTransforms(Camera& renderCam)
{
	for (UID const& uid : m_MeshUIDs)
		((MeshComponent*)Chroma::Scene::GetComponent(uid))->DrawUpdateTransforms(renderCam);
}

Shader* Model::GetShader()
{
	for (UID const& uid : m_MeshUIDs)
		return ((MeshComponent*)Chroma::Scene::GetComponent(uid))->GetShader();
}

void Model::SetShader(Shader* const& newShader)
{
	for (UID const& uid : m_MeshUIDs)
		((MeshComponent*)Chroma::Scene::GetComponent(uid))->SetShader(newShader);
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

// TEXTURES
void Model::AddTexture(Texture texture_val)
{
	for (UID const& uid : m_MeshUIDs)
		((MeshComponent*)Chroma::Scene::GetComponent(uid))->AddTexture(texture_val);
}

// UNIFORMS
void Model::SetMat4(std::string name, glm::mat4 value)
{
	for (UID const& uid : m_MeshUIDs)
		((MeshComponent*)Chroma::Scene::GetComponent(uid))->SetMat4(name, value);
}

void Model::SetInt(std::string name, int value)
{
	for (UID const& uid : m_MeshUIDs)
		((MeshComponent*)Chroma::Scene::GetComponent(uid))->SetInt(name, value);
}

void Model::SetFloat(std::string name, float value)
{
	for (UID const& uid : m_MeshUIDs)
		((MeshComponent*)Chroma::Scene::GetComponent(uid))->SetFloat(name, value);
}

void Model::SetJointUniforms(Shader& skinnedShader)
{
	for (UID const& uid : m_MeshUIDs)
		((SkinnedMesh*)Chroma::Scene::GetComponent(uid))->SetJointUniforms(skinnedShader);
}

//Skeleton* Model::GetSkeleton()
//{
//	return ((SkinnedMesh*)m_Meshes[0])->GetSkeleton();
//}


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

	for (MeshData& meshData : Chroma::ModelLoader::Load(path))
	{
		if (meshData.isSkinned)
		{
			m_IsSkinned = true;
			SkinnedMesh* newSkinnedMesh = new SkinnedMesh(meshData);
			Chroma::Scene::AddMeshComponent(newSkinnedMesh);
			m_MeshUIDs.push_back(newSkinnedMesh->GetUID());
		}
		else
		{
			StaticMesh* newStaticMesh = new StaticMesh(meshData);
			Chroma::Scene::AddMeshComponent(newStaticMesh);
			m_MeshUIDs.push_back(newStaticMesh->GetUID());

		}
	}

}


Model::~Model()
{
}