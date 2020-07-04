#include "AnimConstraintComponent.h"
#include <entity/Entity.h>
#include <component/AnimationComponent.h>
#include <model/SkinnedMesh.h>


namespace Chroma
{
	void AnimConstraintComponent::Init()
	{
		// Set Type
		m_Type = Type::Component::kAnimationConstraintComponent;

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
			AnimationComponent* animComponent = static_cast<AnimationComponent*>(Scene::GetComponent(animationComponentUID));
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
				if (static_cast<MeshComponent*>(Scene::GetComponent(meshComponentUID))->GetIsSkinned())
				{
					return static_cast<SkinnedMesh*>(Scene::GetComponent(meshComponentUID))->GetSkeleton();
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
}
