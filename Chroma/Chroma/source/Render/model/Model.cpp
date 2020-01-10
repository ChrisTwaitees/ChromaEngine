#include "Model.h"
#include <texture/stb_image.h>
#include <model/StaticMesh.h>
#include <model/SkinnedMesh.h>


// DRAW
void Model::Draw(Shader &shader)
{
	for (MeshComponent*& mesh : m_Meshes)
		mesh->Draw(shader);
}

void Model::Draw(Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix)
{
	for (MeshComponent*& mesh : m_Meshes)
			mesh->Draw(RenderCamera, Lights, transformMatrix);
}

void Model::Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix)
{
	for (MeshComponent*& mesh : m_Meshes)
		mesh->Draw(shader, RenderCamera, Lights, transformMatrix);
}

void Model::DrawUpdateMaterials(Shader& shader)
{
	for (MeshComponent*& mesh : m_Meshes)
		mesh->DrawUpdateMaterials(shader);
}

void Model::DrawUpdateTransforms(Camera& renderCam, glm::mat4& modelMatrix)
{
	for (MeshComponent* mesh : m_Meshes)
		mesh->DrawUpdateTransforms(renderCam, modelMatrix);
}

void Model::SetShader(Shader* const& newShader)
{
	for (MeshComponent* mesh : m_Meshes)
		mesh->SetShader(newShader);
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
	for (MeshComponent* mesh : m_Meshes)
		mesh->AddTexture(texture_val);
}

// UNIFORMS
void Model::SetMat4(std::string name, glm::mat4 value)
{
	for (MeshComponent* mesh : m_Meshes)
		mesh->SetMat4(name, value);
}

void Model::SetInt(std::string name, int value)
{
	for (MeshComponent* mesh : m_Meshes)
		mesh->SetInt(name, value);
}

void Model::SetFloat(std::string name, float value)
{
	for (MeshComponent* mesh : m_Meshes)
		mesh->SetFloat(name, value);
}

void Model::SetJointUniforms(Shader& skinnedShader)
{
	for (MeshComponent* mesh : m_Meshes)
	{
		((SkinnedMesh*)mesh)->SetJointUniforms(skinnedShader);
	}	
}

Skeleton* Model::GetSkeleton()
{
	return ((SkinnedMesh*)m_Meshes[0])->GetSkeleton();
}

void Model::SetScale(float const& newScale)
{
	((SkinnedMesh*)m_Meshes[0])->SetScale(newScale);
}

void Model::SetTranslation(glm::vec3 const& newPosition)
{
	((SkinnedMesh*)m_Meshes[0])->SetTranslation(newPosition);
}

void Model::SetRotation(glm::quat const& newRotation)
{
	((SkinnedMesh*)m_Meshes[0])->SetRotation(newRotation);
}

void Model::CalculateBBox()
{
	// collecting all bboxes within mesh components of entity and returning overall
	std::vector<std::pair<glm::vec3, glm::vec3>> bboxes;
	for (MeshComponent*& mesh : m_Meshes)
		bboxes.push_back(mesh->GetBBox());

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
			m_Meshes.push_back(new SkinnedMesh(meshData));
		}
		else
			m_Meshes.push_back(new StaticMesh(meshData));
	}

}


Model::~Model()
{
	for (MeshComponent* mesh : m_Meshes)
		delete mesh;
}