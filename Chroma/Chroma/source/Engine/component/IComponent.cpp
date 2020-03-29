#include "IComponent.h"
#include <scene/Scene.h>

IEntity* IComponent::GetParentEntity() const
{
	return Chroma::Scene::GetEntity(m_ParentEntityUID);
}

UID IComponent::GetParentEntityUID() const
{
	return Chroma::Scene::GetEntity(m_ParentEntityUID)->GetUID();
}

std::string IComponent::GetParentEntityName() const
{
	return GetParentEntity()->GetName();
}

IComponent::IComponent()
{
	m_UID = UID();
}
