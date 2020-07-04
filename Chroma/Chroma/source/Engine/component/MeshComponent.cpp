#include "MeshComponent.h"
#include <entity/IEntity.h>
#include <scene/Scene.h>
#include <resources/ModelLoader.h>


namespace Chroma
{
	MeshComponent::MeshComponent()
	{
		Init();
	}


	MeshComponent::~MeshComponent()
	{
		Destroy();
	}

	void MeshComponent::RebuildTransform()
	{
		m_Transform = Math::BuildMat4(m_Translation, m_Rotation, m_Scale);
	}

	void MeshComponent::CalculateBBox()
	{
		// calculate new min and max bbox
		glm::vec3 newMinBBox(99999.00, 99999.00, 99999.00);
		glm::vec3 newMaxBBox(0.0, 0.0, 0.0);

		if (m_MeshData.isSkinned)
		{
			for (ChromaVertex& vert : m_MeshData.skinnedVerts)
			{
				newMinBBox = glm::min(newMinBBox, vert.m_position);
				newMaxBBox = glm::max(newMaxBBox, vert.m_position);
			}
		}
		else
		{
			for (ChromaVertex& vert : m_MeshData.verts)
			{
				newMinBBox = glm::min(newMinBBox, vert.m_position);
				newMaxBBox = glm::max(newMaxBBox, vert.m_position);
			}
		}
		// re-establishing min and max bboxes
		m_BBoxMin = newMinBBox;
		m_BBoxMax = newMaxBBox;
	}

	void MeshComponent::CalculateCentroid()
	{
		m_Centroid = (m_BBoxMin - m_BBoxMax) * glm::vec3(0.5);
	}


	void MeshComponent::ProcessRenderFlags()
	{
		Chroma::Scene::ProcessMeshComponentRenderFlags(this);
	}


	void MeshComponent::Init()
	{
		// Set Type
		m_Type = Type::Component::kMeshComponent ;

		CMPNT_INITIALIZED
	}

	void MeshComponent::SetupMesh()
	{
		CHROMA_INFO("Mesh Component : {}, Setting Up.", m_UID.m_Data);
	}

	void MeshComponent::LoadFromFile(const std::string& sourcePath)
	{
		CHROMA_INFO("Mesh Component : {0}, Loading from source path : {1}.", m_UID.m_Data, sourcePath);
	}

	void MeshComponent::Update()
	{
	}

	void MeshComponent::Destroy()
	{
		CMPNT_DESTROYED
	}

	void MeshComponent::Serialize(ISerializer*& serializer)
	{
		CMPNT_SERIALIZE_BEGIN
		// Properties
		// Transform
		serializer->AddProperty("m_Translation", &m_Translation);
		serializer->AddProperty("m_Rotation", &m_Rotation);
		serializer->AddProperty("m_Scale", &m_Scale);

		// File Properties
		serializer->AddProperty("m_SourcePath", &m_MeshData.sourcePath);

		// Material 
		m_Material.Serialize(serializer);

	}

	void MeshComponent::CleanUp()
	{
		CHROMA_INFO("Mesh Component : {}, Cleaned Up.", m_UID.m_Data);
	}

	void MeshComponent::SetTransform(glm::mat4 const& newTransformMat)
	{
		m_Transform = newTransformMat;
		m_Scale = Math::GetScale(newTransformMat);
		m_Translation = Math::GetTranslation(newTransformMat);
		m_Rotation = Math::GetQuatRotation(newTransformMat);
	}

	glm::vec3 MeshComponent::GetWSTranslation()
	{
		return GetParentEntity()->GetTranslation() + m_Translation;
	}

	std::pair<glm::vec3, glm::vec3> MeshComponent::GetBBox()
	{
		glm::vec3 wsTranslation = GetParentEntity()->GetTranslation();
		return std::make_pair(m_MeshData.bboxMin + wsTranslation, m_MeshData.bboxMax + wsTranslation);
	}

	glm::vec3 MeshComponent::GetCentroid()
	{
		return GetParentEntity()->GetTranslation() + m_MeshData.centroid;
	}

	glm::mat4 MeshComponent::GetWorldTransform()
	{
		return GetParentEntity()->GetTransform() * GetTransform();
	}

	void MeshComponent::SetMaterial(const Material& newMaterial)
	{
		// Set Material
		m_Material = newMaterial;

		// Refilter RenderPaths according to newMaterial Settings
		ProcessRenderFlags();
	}
}
