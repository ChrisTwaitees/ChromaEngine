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


	std::map<std::string, Type::DataType> Type::m_DataTypeMap =
	{
		{"<NULL>",                        Type::DataType::kNull},
	};

	std::map<std::string, Type::EditorProperty> Type::m_EditorPropertiesMap =
	{
		{"<NULL>",                        Type::EditorProperty::kNullEditorProperty},
		{"kSourcePathProperty",           Type::EditorProperty::kSourcePathProperty},
		{"kFloatPropery",                 Type::EditorProperty::kFloatPropery},
		{"kIntProperty",                  Type::EditorProperty::kIntProperty},
		{"kStringProperty",               Type::EditorProperty::kStringProperty},
		{"kMaterialProperty",             Type::EditorProperty::kMaterialProperty},
		{"kMaterialTextureProperty",      Type::EditorProperty::kMaterialTextureProperty},
		{"kMaterialUniformProperty",      Type::EditorProperty::kMaterialUniformProperty}
	};

	std::map<std::string, Type::Texture> Type::m_TextureTypeMap =
	{
		{"<NULL>",                        Type::Texture::kNullTexture},
		{"kAlbedo",						  Type::Texture::kAlbedo},
		{"kNormal",						  Type::Texture::kNormal},
		{"kMetRoughAO",                   Type::Texture::kMetRoughAO},
		{"kTranslucency",                 Type::Texture::kTranslucency},
		{"kMetalness",					  Type::Texture::kMetalness},
		{"kRoughness",					  Type::Texture::kRoughness},
		{"kAO",							  Type::Texture::kAO},
		{"kHDR",						  Type::Texture::kHDR},
		{"k3D",							  Type::Texture::k3D}
	};

	std::map<std::string, Type::Light> Type::m_LightTypeMap =
	{
		{"<NULL>",                        Type::Light::kNullLight},
		{"kPointLight",					  Type::Light::kPointLight},
		{"kDirectionalLight",			  Type::Light::kDirectionalLight},
		{"kSpotLight",                    Type::Light::kSpotLight},
		{"kSunlight",                     Type::Light::kSunlight}
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

std::string Chroma::Type::GetName(DataType dataType)
{
	return std::string();
}

std::string Chroma::Type::GetName(EditorProperty editorPropertyType)
{
	return std::string();
}

std::string Chroma::Type::GetName(Texture textureType)
{
	switch (textureType)
	{
	case(Texture::kAlbedo):
	{
		return "kAlbedo";
		break;
	}
	case(Texture::kNormal):
	{
		return "kNormal";
		break;
	}
	case(Texture::kMetRoughAO):
	{
		return "kMetRoughAO";
		break;
	}
	case(Texture::kTranslucency):
	{
		return "kTranslucency";
		break;
	}
	case(Texture::kHDR):
	{
		return "kHDR";
		break;
	}
	case(Texture::kMetalness):
	{
		return "kMetalness";
		break;
	}
	case(Texture::kAO):
	{
		return "kAO";
		break;
	}
	case(Texture::kRoughness):
	{
		return "kRoughness";
		break;
	}
	default:
	{
		CHROMA_ERROR("Unsupported Texture Type!");
		return "Unsupported Texture Type!";
		break;
	}
	}
}

std::string Chroma::Type::GetName(Light lightType)
{
	switch (lightType)
	{
	case(Light::kNullLight):
	{
		return "<NULL>";
		break;
	}
	case(Light::kPointLight):
	{
		return "kPointLight";
		break;
	}
	case(Light::kDirectionalLight):
	{
		return "kDirectionalLight";
		break;
	}
	case(Light::kSpotLight):
	{
		return "kSpotLight";
		break;
	}
	case(Light::kSunlight):
	{
		return "kSunlight";
		break;
	}
	default:
	{
		CHROMA_ERROR("Unsupported Light Type!");
		return "Unsupported Light Type!";
		break;
	}
	}
}
