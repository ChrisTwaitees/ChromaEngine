#include "JSONScene.h"
#include <scene/Scene.h>

void JSONScene::AddNewEntity(ISerializer*& serialized)
{
	rapidjson::Value newEntity(rapidjson::kObjectType);
	// Get Type Name
	std::string entityType = Chroma::Type::GetName(serialized->m_EntityType);
	rapidjson::Value entityTypeName(entityType.c_str(), m_Document.GetAllocator());
	// Get UID
	unsigned int uidKey = serialized->m_UID.m_Data;
	// Serialize Entity
	SerializeEntity(serialized, newEntity);

	// Check if entity m_Type already created
	rapidjson::Value::ConstMemberIterator itr = GetEntities().FindMember(entityTypeName);
	if (itr == GetComponents().MemberEnd())
	{
		rapidjson::Value newEntityTypeObject(rapidjson::kObjectType);
		GetEntities().AddMember(entityTypeName, newEntityTypeObject, m_Document.GetAllocator());
	}
	rapidjson::Value uidValue(std::to_string(uidKey).c_str(), m_Document.GetAllocator());
	GetEntities()[entityType.c_str()].AddMember(uidValue, newEntity, m_Document.GetAllocator());
}

void JSONScene::AddNewComponent(ISerializer*& serialized)
{
	rapidjson::Value newComponent(rapidjson::kObjectType);
	// Get Type Name
	std::string componentType = Chroma::Type::GetName(serialized->m_ComponentType);
	rapidjson::Value componentTypeName(componentType.c_str(), m_Document.GetAllocator());
	// Get UID
	unsigned int uidKey = serialized->m_UID.m_Data;
	// Serialize Component
	SerializeComponent(serialized, newComponent);

	// Check if entity m_Type already created
	rapidjson::Value::ConstMemberIterator itr = GetComponents().FindMember(componentTypeName);
	if (itr == GetComponents().MemberEnd())
	{
		rapidjson::Value newComponentTypeObject(rapidjson::kObjectType);
		GetComponents().AddMember(componentTypeName, newComponentTypeObject, m_Document.GetAllocator());
	}

	rapidjson::Value uidValue(std::to_string(uidKey).c_str(), m_Document.GetAllocator());
	GetComponents()[componentType.c_str()].AddMember(uidValue, newComponent, m_Document.GetAllocator());

}

void JSONScene::AddIBL(IBL* ibl)
{
	rapidjson::Value newIBL(rapidjson::kObjectType);
	rapidjson::Value newIBLKey(CHROMA_IBL_KEY, m_Document.GetAllocator());
	rapidjson::Value newiblSourcePath(ibl->GetSourcePath().c_str(), m_Document.GetAllocator());
	GetLevel().AddMember(newIBLKey, newiblSourcePath, m_Document.GetAllocator());
}

rapidjson::Value& JSONScene::GetRoot()
{
	return m_Document[CHROMA_ROOT];
}

rapidjson::Value& JSONScene::GetLevel()
{
	return m_Document[CHROMA_ROOT][CHROMA_LEVEL];
}

rapidjson::Value& JSONScene::GetEntities()
{
	return m_Document[CHROMA_ROOT][CHROMA_LEVEL][CHROMA_ENTITIES];
}

rapidjson::Value& JSONScene::GetComponents()
{
	return m_Document[CHROMA_ROOT][CHROMA_LEVEL][CHROMA_COMPONENTS];
}

rapidjson::Value& JSONScene::GetIBL()
{
	return GetLevel()[CHROMA_IBL_KEY];
}

JSONScene::JSONScene()
{
	// Build Root Structure

	// Instantiate Root to Object
	m_Document.SetObject();

	// add entities to levels
	m_LevelObject.AddMember(CHROMA_ENTITIES, m_EntitiesObject, m_Document.GetAllocator());

	// add components to levels
	m_LevelObject.AddMember(CHROMA_COMPONENTS, m_ComponentsObject, m_Document.GetAllocator());

	// add levels to Root
	m_RootObject.AddMember(CHROMA_LEVEL, m_LevelObject, m_Document.GetAllocator());

	// add root to Document
	m_Document.AddMember(CHROMA_ROOT, m_RootObject, m_Document.GetAllocator());

}

JSONScene::JSONScene(const char* loadScenePath)
{
	Load(loadScenePath);
}

void JSONScene::SerializeEntity(ISerializer*& serialized, rapidjson::Value& jsonValue)
{
	// Serialized generic types
	SerializeTypes(serialized, jsonValue);

	// Serialize Children Component UIDs in Type Categories
	for (UID& componentUID : Chroma::Scene::GetEntity(serialized->m_UID)->GetComponentUIDs())
	{
		std::string componentType = Chroma::Scene::GetComponent(componentUID)->GetTypeName();
		rapidjson::Value componentTypeName(componentType.c_str(), m_Document.GetAllocator());

		// Check if component array already created
		rapidjson::Value::ConstMemberIterator itr = jsonValue.FindMember(componentTypeName);
		if (itr == jsonValue.MemberEnd())
		{
			rapidjson::Value newComponentTypeArray(rapidjson::kArrayType);
			jsonValue.AddMember(componentTypeName, newComponentTypeArray, m_Document.GetAllocator());
		}
		// add component uid
		jsonValue[componentType.c_str()].PushBack(componentUID.m_Data, m_Document.GetAllocator());
	}

}

void JSONScene::SerializeComponent(ISerializer*& serialized, rapidjson::Value& jsonValue)
{
	// Serialized generic types
	SerializeTypes(serialized, jsonValue);

	// Component Specific Serialization 
	if (Chroma::Type::IsMeshComponent( static_cast<IComponent*>(Chroma::Scene::GetComponent(serialized->m_UID))->GetType()))
		SerializeMaterialTypes(serialized, jsonValue);
}

void JSONScene::SerializeMaterialTypes(ISerializer*& serialized, rapidjson::Value& jsonValue)
{
	// Material Key
	rapidjson::Value materialKey(CHROMA_MATERIAL_KEY, m_Document.GetAllocator());

	// Check if JsonValue Contains MaterialKey add Object if not
	rapidjson::Value::ConstMemberIterator itr = jsonValue.FindMember(materialKey);
	if (itr == jsonValue.MemberEnd())
	{
		rapidjson::Value materialObject{ rapidjson::kObjectType };
		jsonValue.AddMember(materialKey, materialObject, m_Document.GetAllocator());
	}

	// String Properties
	for (auto& str : serialized->m_ConstStringProperties)
	{
		rapidjson::Value stringKey(str.first.m_Name, m_Document.GetAllocator());
		rapidjson::Value stringValue(str.second.c_str(), m_Document.GetAllocator());

		// Check which value type
		if (str.first.m_EditorProperty.m_Type == Chroma::Type::EditorProperty::kMaterialTextureProperty)
		{
			// Texture Key
			rapidjson::Value texturesKey(CHROMA_MATERIAL_TEXTURES_KEY, m_Document.GetAllocator());
			// Check if JsonValue Contains Texture Key add Object if not
			rapidjson::Value::ConstMemberIterator itr = jsonValue[CHROMA_MATERIAL_KEY].FindMember(texturesKey);
			if (itr == jsonValue[CHROMA_MATERIAL_KEY].MemberEnd())
			{
				rapidjson::Value texturesObject{ rapidjson::kObjectType };
				jsonValue[CHROMA_MATERIAL_KEY].AddMember(texturesKey, texturesObject, m_Document.GetAllocator());
			}
			jsonValue[CHROMA_MATERIAL_KEY][CHROMA_MATERIAL_TEXTURES_KEY].AddMember(stringKey, stringValue, m_Document.GetAllocator());
		}
		else
			continue;
	}
	

	//jsonValue[CHROMA_MATERIAL_KEY].AddMember();
}

void JSONScene::SerializeTypes(ISerializer*& serialized, rapidjson::Value& jsonValue)
{
	// UID Properties
	for (auto& uid : serialized->m_UIDs)
	{
		rapidjson::Value stringKey(uid.first.m_Name, m_Document.GetAllocator());
		rapidjson::Value stringValue(std::to_string(uid.second.m_Data).c_str(), m_Document.GetAllocator());

		// Check which value type
		if ( Chroma::Type::IsMaterialEditorProperty(uid.first.m_EditorProperty.m_Type))
			continue;
		else
			jsonValue.AddMember(stringKey, stringValue, m_Document.GetAllocator());
	}

	// String Properties
	for (auto& str : serialized->m_StringProperties)
	{
		rapidjson::Value stringKey(str.first.m_Name, m_Document.GetAllocator());
		rapidjson::Value stringValue(str.second->c_str(), m_Document.GetAllocator());
		
		// Check which value type
		if (Chroma::Type::IsMaterialEditorProperty(str.first.m_EditorProperty.m_Type))
			continue;
		else
			jsonValue.AddMember(stringKey, stringValue, m_Document.GetAllocator());
	}

	// Const String Properties
	for (auto& str : serialized->m_ConstStringProperties)
	{
		rapidjson::Value stringKey(str.first.m_Name, m_Document.GetAllocator());
		rapidjson::Value stringValue(str.second.c_str(), m_Document.GetAllocator());

		// Check which value type
		if (Chroma::Type::IsMaterialEditorProperty(str.first.m_EditorProperty.m_Type))
			continue;
		else
			jsonValue.AddMember(stringKey, stringValue, m_Document.GetAllocator());
	}

	// Float Properties
	for (auto& floatVal : serialized->m_FloatProperties)
	{
		rapidjson::Value floatKey(floatVal.first.m_Name, m_Document.GetAllocator());
		rapidjson::Value floatValue(rapidjson::kNumberType);
		floatValue.SetFloat(*floatVal.second);
		
		// Check which value type
		if (Chroma::Type::IsMaterialEditorProperty(floatVal.first.m_EditorProperty.m_Type))
			continue;
		else
			jsonValue.AddMember(floatKey, floatValue, m_Document.GetAllocator());
	}

	// Int Properties
	for (auto& intVal : serialized->m_IntProperties)
	{
		rapidjson::Value intKey(intVal.first.m_Name, m_Document.GetAllocator());
		rapidjson::Value intValue(rapidjson::kNumberType);
		intValue.SetInt(*intVal.second);

		// Check which value type
		if (Chroma::Type::IsMaterialEditorProperty(intVal.first.m_EditorProperty.m_Type))
			continue;
		else
			jsonValue.AddMember(intKey, intValue, m_Document.GetAllocator());
	}

	// uInt Properties
	for (auto& uIntVal : serialized->m_UIntProperties)
	{
		rapidjson::Value intKey(uIntVal.first.m_Name, m_Document.GetAllocator());
		rapidjson::Value intValue(rapidjson::kNumberType);
		intValue.SetInt(*uIntVal.second);

		// Check which value type
		if (Chroma::Type::IsMaterialEditorProperty(uIntVal.first.m_EditorProperty.m_Type))
			continue;
		else
			jsonValue.AddMember(intKey, intValue, m_Document.GetAllocator());
	}

	// Vec2 Properties
	for (auto& vec2 : serialized->m_Vec2Properties)
	{
		rapidjson::Value vec2Key(vec2.first.m_Name, m_Document.GetAllocator());
		rapidjson::Value vec2Value(rapidjson::kArrayType);
		vec2Value.PushBack(vec2.second->x, m_Document.GetAllocator());
		vec2Value.PushBack(vec2.second->y, m_Document.GetAllocator());

		// Check which value type
		if (Chroma::Type::IsMaterialEditorProperty(vec2.first.m_EditorProperty.m_Type))
			continue;
		else
			jsonValue.AddMember(vec2Key, vec2Value, m_Document.GetAllocator());
	}

	// Vec3 Properties
	for (auto& vec3 : serialized->m_Vec3Properties)
	{
		rapidjson::Value vec3Key(vec3.first.m_Name, m_Document.GetAllocator());
		rapidjson::Value vec3Value(rapidjson::kArrayType);
		vec3Value.PushBack(vec3.second->x, m_Document.GetAllocator());
		vec3Value.PushBack(vec3.second->y, m_Document.GetAllocator());
		vec3Value.PushBack(vec3.second->z, m_Document.GetAllocator());

		// Check which value type
		if (Chroma::Type::IsMaterialEditorProperty(vec3.first.m_EditorProperty.m_Type))
			continue;
		else
			jsonValue.AddMember(vec3Key, vec3Value, m_Document.GetAllocator());
	}

	// Vec4 Properties
	for (auto& vec4 : serialized->m_Vec4Properties)
	{
		rapidjson::Value vec4Key(vec4.first.m_Name, m_Document.GetAllocator());
		rapidjson::Value vec4Value(rapidjson::kArrayType);
		vec4Value.PushBack(vec4.second->x, m_Document.GetAllocator());
		vec4Value.PushBack(vec4.second->y, m_Document.GetAllocator());
		vec4Value.PushBack(vec4.second->z, m_Document.GetAllocator());
		vec4Value.PushBack(vec4.second->w, m_Document.GetAllocator());

		// Check which value type
		if (Chroma::Type::IsMaterialEditorProperty(vec4.first.m_EditorProperty.m_Type))
			continue;
		else
			jsonValue.AddMember(vec4Key, vec4Value, m_Document.GetAllocator());
	}

	// Quat Properties
	for (auto& quat : serialized->m_QuatProperties)
	{
		rapidjson::Value quatKey(quat.first.m_Name, m_Document.GetAllocator());
		rapidjson::Value quatValue(rapidjson::kArrayType);
		quatValue.PushBack(quat.second->x, m_Document.GetAllocator());
		quatValue.PushBack(quat.second->y, m_Document.GetAllocator());
		quatValue.PushBack(quat.second->z, m_Document.GetAllocator());
		quatValue.PushBack(quat.second->w, m_Document.GetAllocator());
		jsonValue.AddMember(quatKey, quatValue, m_Document.GetAllocator());
	}


}



