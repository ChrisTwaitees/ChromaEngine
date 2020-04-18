#include "Types.h"


namespace Chroma
{
	std::map<std::string, Type::Component> Type::m_ComponentMap =
	{
		{ "<NULL>",                         Type::Component::kNullComponent},
		{ "kIComponent",                    Type::Component::kIComponent},
		{ "kStateMachineComponent",         Type::Component::kStateMachineComponent},
		{ "kAnimationComponent",            Type::Component::kAnimationComponent},
		{ "kAnimationConstraintComponent",  Type::Component::kAnimationConstraintComponent},
		{ "kCharacterControllerComponent",  Type::Component::kCharacterControllerComponent},
		{ "kCharacterPhysicsComponent",     Type::Component::kCharacterPhysicsComponent},
		{ "kPhysicsComponent",              Type::Component::kPhysicsComponent},
		{ "kMeshComponent",                 Type::Component::kMeshComponent},
		{ "kUIComponent",                   Type::Component::kUIComponent},
		{ "kLightComponent",                Type::Component::kLightComponent},
		{ "kModelComponent",                Type::Component::kModelComponent},
		{ "kStaticMeshComponent",           Type::Component::kStaticMeshComponent},
		{ "kSkinnedMeshComponent",          Type::Component::kSkinnedMeshComponent}
	};

	std::map<std::string, Type::Entity> Type::m_EntityMap =
	{
		{ "<NULL>",                        Type::Entity::kNullEntity},
		{ "kIEntity",                      Type::Entity::kIEntity},
		{ "kEntity",					   Type::Entity::kEntity}
	};

	std::map<std::string, Type::Serialization> Type::m_SerializationMap =
	{
		{"<NULL>",                        Type::Serialization::kNullSerialization},
		{"kJSON",                         Type::Serialization::kJSON}
	};
}


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
	case(Component::kStaticMeshComponent):
	{
		return "kStaticMeshComponent";
		break;
	}
	case(Component::kSkinnedMeshComponent):
	{
		return "kSkinnedMeshComponent";
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
		return "kEntity";
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

std::string Chroma::Type::GetName(Serialization serializedType)
{
	switch (serializedType)
	{
	case(Serialization::kJSON):
	{
		return "kJSON";
		break;
	}
	default:
	{
		CHROMA_ERROR("Unsupported Serialization Type!");
		return "Unsupported Serialization Type!";
		break;
	}
	}
}
