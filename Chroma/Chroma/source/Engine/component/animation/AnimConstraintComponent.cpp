#include "AnimConstraintComponent.h"
#include <entity/Entity.h>
#include <component/AnimationComponent.h>
#include <model/SkinnedMesh.h>

void AnimConstraintComponent::Init()
{
	// Set Type
	m_Type = Chroma::Type::Component::kAnimationConstraintComponent;

	CMPNT_INITIALIZED
}

void AnimConstraintComponent::Update()
{
	CHROMA_INFO("Updating AnimConstraint Component : {0}", m_UID.m_Data);
}

void AnimConstraintComponent::Destroy()
{
	CMPNT_DESTROYED
}

void AnimConstraintComponent::Serialize(ISerializer*& serializer)
{
	CMPNT_SERIALIZE_BEGIN

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
