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

	IKConstraint& GetConstraint(std::string const& constraintName);

	// Init
	IKAnimConstraint() {};
	~IKAnimConstraint() {};
};

#endif //CHROMA_IK_ANIM_CONSTRAINT_H