#include "Types.h"

std::string Chroma::Type::GetName(Component componentEnum)
{
	switch(componentEnum)
	{
	case(Component::kIComponent) :
	{
		return "kIComponent";
		break;
	}
	case(Component::kStateMachineComponent):
	{
		return "kStateMachineComponent";
		break;
	}
	case(Component::kAnimationComponent):
	{
		return "kAnimationComponent";
		break;
	}
	case(Component::kAnimationConstraintComponent):
	{
		return "kAnimationConstraintComponent";
		break;
	}
	case(Component::kCharacterControllerComponent):
	{
		return "kCharacterControllerComponent";
		break;
	}
	case(Component::kCharacterPhysicsComponent):
	{
		return "kCharacterPhysicsComponent";
		break;
	}
	case(Component::kPhysicsComponent):
	{
		return "kPhysicsComponent";
		break;
	}
	case(Component::kMeshComponent):
	{
		return "kMeshComponent";
		break;
	}
	case(Component::kUIComponent):
	{
		return "kUIComponent";
		break;
	}
	case(Component::kLightComponent):
	{
		return "kLightComponent";
		break;
	}
	case(Component::kModelComponent):
	{
		return "kModelComponent";
		break;
	}
	default:
	{
		CHROMA_ERROR("Unsupported Component Type!");
		return "Unsupported Component Type!";
		break;
	}
	}
}

std::string Chroma::Type::GetName(Entity entityEnum)
{
	switch (entityEnum)
	{
	case(Entity::kIEntity):
	{
		return "kIEntity";
		break;
	}
	case(Entity::kEntity):
	{
		return "kIEntity";
		break;
	}
	default:
	{
		CHROMA_ERROR("Unsupported Entity Type!");
		return "Unsupported Entity Type!";
		break;
	}
	}
}
