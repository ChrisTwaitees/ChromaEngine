#include "CharacterPhysicsComponent.h"
#include <physics/PhysicsEngine.h>

void CharacterPhysicsComponent::Init()
{
	// Set Type
	m_Type = Chroma::Type::Component::kCharacterPhysicsComponent;

	// build rigidBody
	BuildRigidBody();

	// add rigid body to physics world
	Chroma::Physics::AddBodyToWorld(m_RigidBody);

	// debug
	CMPNT_INITIALIZED;
}
