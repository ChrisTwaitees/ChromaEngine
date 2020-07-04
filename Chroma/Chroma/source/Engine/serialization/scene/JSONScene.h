#ifndef CHROMA_JSON_SCENE_H
#define CHROMA_JSON_SCENE_H

#include <serialization/formats/Json.h>
#include <common/PrecompiledHeader.h>
#include <serialization/ISerializer.h>
#include <scene/Scene.h>

namespace Chroma
{
	class JSONScene : public JSON
	{
	public:
		// ECS
		void AddNewEntity(ISerializer*& serialized);
		void AddNewComponent(ISerializer*& serialized);

		// Scene Entities
		void AddIBL(IBL* ibl);

		rapidjson::Value& GetRoot();
		rapidjson::Value& GetLevel();
		rapidjson::Value& GetEntities();
		rapidjson::Value& GetComponents();
		rapidjson::Value& GetIBL();

		JSONScene();
		JSONScene(const char* loadScenePath);
		~JSONScene() {};

	private:
		rapidjson::Value m_RootObject{ rapidjson::kObjectType };
		rapidjson::Value m_LevelObject{ rapidjson::kObjectType };
		rapidjson::Value m_EntitiesObject{ rapidjson::kObjectType };
		rapidjson::Value m_ComponentsObject{ rapidjson::kObjectType };

		void SerializeTypes(ISerializer*& serialized, rapidjson::Value& jsonValue);
		void SerializeEntity(ISerializer*& serialized, rapidjson::Value& jsonValue);
		void SerializeComponent(ISerializer*& serialized, rapidjson::Value& jsonValue);
		void SerializeMaterialTypes(ISerializer*& serialized, rapidjson::Value& jsonValue);
	};
}

#define CHROMA_ROOT                  "Root"
#define CHROMA_LEVEL                 "Level"
#define CHROMA_ENTITIES              "Entities"
#define CHROMA_COMPONENTS            "Components"
#define CHROMA_IBL_KEY	             "IBL"
#define CHROMA_MATERIAL_KEY          "Material"
#define CHROMA_MATERIAL_TEXTURES_KEY "Textures"
#define CHROMA_MATERIAL_PROPERTY_FLAGS 	Chroma::Type::EditorProperty::kMaterialProperty | Chroma::Type::EditorProperty::kMaterialTextureProperty | Chroma::Type::EditorProperty::kMaterialUniformProperty | Chroma::Type::EditorProperty::kMaterialUniformColorProperty
#endif