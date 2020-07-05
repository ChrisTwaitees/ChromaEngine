#ifndef CHROMA_IK_ANIM_CONSTRAINT_H
#define CHROMA_IK_ANIM_CONSTRAINT_H

#include <component/animation/AnimConstraintComponent.h>

namespace Chroma
{
	class IKAnimConstraint : public AnimConstraintComponent
	{
	public:
		// Component Functions
		virtual void Init();
		virtual void OnUpdate();
		virtual void Destroy();

		// Serialization
		virtual void Serialize(ISerializer*& serializer);

		// Effectors
		void SetEffectorWorldPos(std::string const& constraintName, glm::vec3 const& worldPos);
		void SetEffectorModelPos(std::string const& constraintName, glm::vec3 const& modelPos);

		// Solver
		inline void SetIterations(unsigned int const& iterations) { m_Iterations = iterations; };

		IKConstraint& GetConstraint(std::string const& constraintName);

		// Init
		IKAnimConstraint() {};
		~IKAnimConstraint() {};
	private:
		void SolveIK(IKConstraint const& ik);
		glm::quat GetJointOrientationWS(unsigned int const& jointID);

		unsigned int m_Iterations{ 10 };
		const float m_DeltaThreshold{ 0.01f };
	};
}

#endif //CHROMA_IK_ANIM_CONSTRAINT_H