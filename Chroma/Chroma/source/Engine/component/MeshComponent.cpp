#include "MeshComponent.h"
#include <entity/IEntity.h>

MeshComponent::MeshComponent()
{
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

void MeshComponent::SetTransform(glm::mat4 const& newTransformMat)
{
	m_Transform = newTransformMat;
	m_Scale = Chroma::Math::GetScale(newTransformMat);
	m_Translation = Chroma::Math::GetTranslation(newTransformMat);
	m_Rotation = Chroma::Math::GetQuatRotation(newTransformMat);
}

glm::mat4 MeshComponent::GetWorldTransform()
{
	return GetParentEntity()->GetTransform()* m_Transform;
}
