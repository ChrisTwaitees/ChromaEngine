#ifndef CHROMA_IK_COMPONENT_H
#define CHROMA_IK_COMPONENT_H

#include <component/IComponent.h>
#include <animation/Skeleton.h>


namespace Chroma
{
	class Animator;
	class AnimConstraintComponent : public IComponent
	{
	public:
		// Component Functions
		virtual void Init();
		virtual void OnUpdate();
		virtual void Destroy();

		// Serialization
		virtual void Serialize(ISerializer*& serializer);

		// Funcs
		inline bool GetIsActive() { return m_IsActive; };
		inline void SetIsActive(bool const& isActive) { m_IsActive = isActive; };

		Animator& GetAnimator();
		Skeleton* GetSkeleton();
		Constraint GetConstraint(const char* name);

		AnimConstraintComponent() {};
		~AnimConstraintComponent() {};

	protected:
		bool m_IsActive{ true };
		float m_ActivationAmount{ 1.0f };


	};
}

#endif // CHROMA_IK_COMPONENT_H