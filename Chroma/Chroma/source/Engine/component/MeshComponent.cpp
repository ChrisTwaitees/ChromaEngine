#include "MeshComponent.h"
#include <entity/IEntity.h>

MeshComponent::MeshComponent()
{
	Init();
	m_IsRenderable = true;
	m_IsLit = true;
	m_CastShadows = true;
	m_IsTransparent = false;
}


MeshComponent::~MeshComponent()
{
}

void MeshComponent::RebuildTransform()
{
	m_Transform = Chroma::Math::BuildMat4(m_Translation, m_Rotation, m_Scale);
}

void MeshComponent::SerializeTextures(ISerializer*& serializer)
{
	// Textures
	for (Texture& texture : m_Textures)
	{
		switch (texture.m_Type)
		{
		case(Texture::ALBEDO):
		{
			serializer->AddProperty("Texture_ALBEDO", &texture.GetSourcePath());
			break;
		}
		case(Texture::NORMAL):
		{
			serializer->AddProperty("Texture_NORMAL", &texture.GetSourcePath());
			break;
		}
		case(Texture::METROUGHAO):
		{
			serializer->AddProperty("Texture_METROUGHAO", &texture.GetSourcePath());
			break;
		}
		case(Texture::TRANSLUCENCY):
		{
			serializer->AddProperty("Texture_TRANSLUCENCY", &texture.GetSourcePath());
			break;
		}
		}
	}
}

void MeshComponent::DestroyTextures()
{
	for (Texture& texture : m_Textures)
		texture.Destroy();

	m_Textures.clear();
}

void MeshComponent::Init()
{
	// Set Type
	m_Type = Chroma::Type::Component::kMeshComponent ;

	CMPNT_INITIALIZED
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
	serializer->AddProperty("m_SourcePath", &m_SourcePath);

	// Material 
	serializer->AddProperty("m_Material", &m_Material);

}

void MeshComponent::SetTransform(glm::mat4 const& newTransformMat)
{
	m_Transform = newTransformMat;
	m_Scale = Chroma::Math::GetScale(newTransformMat);
	m_Translation = Chroma::Math::GetTranslation(newTransformMat);
	m_Rotation = Chroma::Math::GetQuatRotation(newTransformMat);
}

glm::mat4 MeshComponent::GetWorldTransform()
{
	return GetParentEntity()->GetTransform() * GetTransform();
}
