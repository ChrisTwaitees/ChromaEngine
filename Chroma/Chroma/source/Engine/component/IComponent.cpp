#include "IComponent.h"
#include <scene/Scene.h>

namespace Chroma
{
	IEntity* IComponent::GetParentEntity() const
	{
		return Scene::GetEntity(m_ParentEntityUID);
	}

	UID IComponent::GetParentEntityUID() const
	{
		return Scene::GetEntity(m_ParentEntityUID)->GetUID();
	}

	std::string IComponent::GetParentEntityName() const
	{
		return GetParentEntity()->GetName();
	}

	IComponent::IComponent()
	{
		m_UID = UID();
	}
}
