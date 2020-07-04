#include "CharacterPhysicsComponent.h"
#include <physics/PhysicsEngine.h>


namespace Chroma
{
	void CharacterPhysicsComponent::Init()
	{
		// Set Type
		m_Type = Type::Component::kCharacterPhysicsComponent;

		// build rigidBody
		BuildRigidBody();

		// add rigid body to physics world
		Chroma::Physics::AddBodyToWorld(m_RigidBody);

		// debug
		CMPNT_INITIALIZED;
	}
}
