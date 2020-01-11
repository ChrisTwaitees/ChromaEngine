#include "IComponent.h"
#include <scene/Scene.h>

IEntity* IComponent::GetParentEntity() const
{
	return Chroma::Scene::GetEntity(m_ParentEntityUID);
}

IComponent::IComponent()
{
	m_UID = UID::GenerateNewUID();
}
