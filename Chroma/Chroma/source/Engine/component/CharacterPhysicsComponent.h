#ifndef CHROMA_CHARACTER_PHYSICS_COMPONENT_H
#define CHROMA_CHARACTER_PHYSICS_COMPONENT_H

#include <component/PhysicsComponent.h>


class CharacterPhysicsComponent : public PhysicsComponent
{
public:
	CharacterPhysicsComponent() {};
	~CharacterPhysicsComponent() {};
protected:
	// Serialization
	Chroma::Type::Component m_Type{ Chroma::Type::Component::kCharacterPhysicsComponent };
};

#endif //CHROMA_CHARACTER_PHYSICS_COMPONENT_H