#include "CharacterPhysicsComponent.h"

void CharacterPhysicsComponent::Init()
{
	// Set Type
	m_Type = Chroma::Type::Component::kCharacterPhysicsComponent;

	CMPNT_INITIALIZED;
}
