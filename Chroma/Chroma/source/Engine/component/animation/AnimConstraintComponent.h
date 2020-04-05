#ifndef CHROMA_IK_COMPONENT_H
#define CHROMA_IK_COMPONENT_H

#include <component/IComponent.h>

class AnimConstraintComponent : public IComponent
{
public:
	// Component Functions
	virtual void Init();
	virtual void Update();
	virtual void Destroy();

	// Serialization
	virtual void Serialize(ISerializer*& serializer);
	virtual std::string GetTypeString() const;

	// Funcs
	inline bool GetIsActive() { return m_isActive; };
	inline void SetIsActive(bool const& isActive) { m_isActive = isActive; };


	AnimConstraintComponent() {};
	~AnimConstraintComponent() {};
private:

	bool m_isActive{ true };
	float m_ActivationAmount{ 1.0f };
};

#endif // CHROMA_IK_COMPONENT_H