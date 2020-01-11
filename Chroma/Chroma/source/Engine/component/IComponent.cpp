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

IComponent::IComponent()
{
	m_UID = UID();
}
