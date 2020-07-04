#ifndef CHROMA_CHARACTER_PHYSICS_COMPONENT_H
#define CHROMA_CHARACTER_PHYSICS_COMPONENT_H

#include <component/PhysicsComponent.h>

namespace Chroma
{
	class CharacterPhysicsComponent : public PhysicsComponent
	{
	public:
		void Init() override;


		CharacterPhysicsComponent() {};
		~CharacterPhysicsComponent() {};
	protected:
	};
}

#endif //CHROMA_CHARACTER_PHYSICS_COMPONENT_H