
#ifndef CHROMA_TYPES_H
#define CHROMA_TYPES_H
#include <string>
#include <logging/Log.h>

namespace Chroma
{
	namespace Type
	{
		enum Component {
			kIComponent = 0,
			kStateMachineComponent = 1,
			kAnimationComponent = 2,
			kAnimationConstraintComponent = 3,
			kIKAnimationConstraintComponent = 4,
			kCharacterControllerComponent = 5,
			kCharacterPhysicsComponent = 6, 
			kPhysicsComponent = 7, 
			kMeshComponent = 8,
			kUIComponent = 9,
			kLightComponent = 10

		};

		enum Entity {
			kIEntity = 0,
			kEntity = 1
		};

		std::string GetName(Component componentEnum);

		std::string GetName(Entity entityEnum);

	}



}

#endif //CHROMA_TYPES_H
