#include "AnimConstraintComponent.h"
#include <entity/Entity.h>
#include <component/AnimationComponent.h>
#include <model/SkinnedMesh.h>

void AnimConstraintComponent::Init()
{
	CHROMA_INFO("AnimConstraint Component {0} Initialized!", m_UID.data);
}

void AnimConstraintComponent::Update()
{
	CHROMA_INFO("Updating AnimConstraint Component : {0}", m_UID.data);
}

void AnimConstraintComponent::Destroy()
{
	CHROMA_INFO("AnimConstraint Component {0} Destroyed!", m_UID.data);
}

void AnimConstraintComponent::Serialize(ISerializer*& serializer)
{
	serializer->StartObject(GetTypeString().c_str(), m_UID);
}

std::string AnimConstraintComponent::GetTypeString() const
{
	return "Anim Constraint";
}

Animator& AnimConstraintComponent::GetAnimator()
{
	if (GetParentEntity()->GetAnimationComponentUIDs().size() > 0)
	{
		UID animationComponentUID = GetParentEntity()->GetAnimationComponentUIDs()[0];
		AnimationComponent* animComponent = static_cast<AnimationComponent*>(Chroma::Scene::GetComponent(animationComponentUID));
		return animComponent->GetAnimator();
	}
	else
	{
		CHROMA_ERROR("No Animator found within Entity : {0}", GetParentEntityName());
	}
}

Skeleton* AnimConstraintComponent::GetSkeleton()
{
	if (GetParentEntity()->GetMeshComponentUIDs().size() > 0)
	{
		for (UID const& meshComponentUID : GetParentEntity()->GetMeshComponentUIDs())
		{
			if (static_cast<MeshComponent*>(Chroma::Scene::GetComponent(meshComponentUID))->GetIsSkinned())
			{
				return static_cast<SkinnedMesh*>(Chroma::Scene::GetComponent(meshComponentUID))->GetSkeleton();
			}
		}
	}
	else
	{
		CHROMA_ERROR("No Skeleton found within Entity : {0}", GetParentEntityName());
	}
}

Constraint AnimConstraintComponent::GetConstraint(const char* name)
{
	return Constraint();
}
