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

	// Init
	IKAnimConstraint() {};
	~IKAnimConstraint() {};
};

#endif //CHROMA_IK_ANIM_CONSTRAINT_H