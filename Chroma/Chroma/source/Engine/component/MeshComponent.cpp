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
