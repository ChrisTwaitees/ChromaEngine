#ifndef CHROMA_IK_ANIM_CONSTRAINT_H
#define CHROMA_IK_ANIM_CONSTRAINT_H

#include <component/animation/AnimConstraintComponent.h>

class IKAnimConstraint : public AnimConstraintComponent
{
public:
	// Component Functions
	virtual void Init();
	virtual void Update();
	virtual void Destroy();

	// Serialization
	virtual void Serialize(ISerializer*& serializer);
	virtual std::string GetTypeString() const;

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

	unsigned int m_Iterations{ 10 };
};

#endif //CHROMA_IK_ANIM_CONSTRAINT_H