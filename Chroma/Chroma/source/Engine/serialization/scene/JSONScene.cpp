#include "JSONScene.h"

void JSONScene::AddNewEntity(ISerializer*& serialized)
{
	rapidjson::Value newEntity(rapidjson::kObjectType);
	// Get Type Name
	std::string entityType = Chroma::Type::GetName(serialized->m_EntityType);
	rapidjson::Value entityTypeName(entityType.c_str(), m_Document.GetAllocator());
	// Get UID
	std::string uidKey = serialized->m_UID.data;
	// Serialize Entity
	SerializeEntity(serialized, newEntity);

	// Check if entity type already created
	rapidjson::Value::ConstMemberIterator itr = GetEntities().FindMember(entityTypeName);
	if (itr != GetComponents().MemberEnd())
	{
		GetEntities()[entityType.c_str()].AddMember(rapidjson::StringRef(uidKey.c_str()), newEntity, m_Document.GetAllocator());
	}
	else // Create new entity type dict
	{
		rapidjson::Value newEntityTypeObject(rapidjson::kObjectType);
		GetEntities().AddMember(entityTypeName, newEntityTypeObject, m_Document.GetAllocator());
		GetEntities()[entityType.c_str()].AddMember(rapidjson::StringRef(uidKey.c_str()), newEntity, m_Document.GetAllocator());
	}
}

void JSONScene::AddNewComponent(ISerializer*& serialized)
{
	rapidjson::Value newComponent(rapidjson::kObjectType);
	// Get Type Name
	std::string componentType = Chroma::Type::GetName(serialized->m_ComponentType);
	rapidjson::Value componentTypeName(componentType.c_str(), m_Document.GetAllocator());
	// Get UID
	std::string uidKey = serialized->m_UID.data;
	// Serialize Component
	SerializeComponent(serialized, newComponent);

	// Check if entity type already created
	rapidjson::Value::ConstMemberIterator itr = GetComponents().FindMember(componentTypeName);
	if (itr != GetComponents().MemberEnd())
	{
		GetComponents()[componentType.c_str()].AddMember(rapidjson::StringRef(uidKey.c_str()), newComponent, m_Document.GetAllocator());
	}
	else // Create new entity type dict
	{
		rapidjson::Value newComponentTypeObject(rapidjson::kObjectType);
		GetComponents().AddMember(componentTypeName, newComponentTypeObject, m_Document.GetAllocator());
		GetComponents()[componentType.c_str()].AddMember(rapidjson::StringRef(uidKey.c_str()), newComponent, m_Document.GetAllocator());
	}

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

rapidjson::Value& JSONScene::SerializeEntity(ISerializer*& serialized, rapidjson::Value& jsonValue)
{
	// String Properties
	for (std::pair<const char*, std::string*>&& str : serialized->m_StringProperties)
	{
		rapidjson::Value stringKey(str.first, m_Document.GetAllocator());
		rapidjson::Value stringValue(str.second->c_str(), m_Document.GetAllocator());
		jsonValue.AddMember(stringKey, stringValue, m_Document.GetAllocator());
	}

	// Vec3 Properties
	for (std::pair<const char*, glm::vec3*>&& vec3 : serialized->m_Vec3Properties)
	{
		rapidjson::Value vec3Key(vec3.first, m_Document.GetAllocator());
		rapidjson::Value vec3Value(rapidjson::kArrayType);
		vec3Value.PushBack(vec3.second->x, m_Document.GetAllocator());
		vec3Value.PushBack(vec3.second->y, m_Document.GetAllocator());
		vec3Value.PushBack(vec3.second->z, m_Document.GetAllocator());
		jsonValue.AddMember(vec3Key, vec3Value, m_Document.GetAllocator());
	}

	return jsonValue;
}

rapidjson::Value& JSONScene::SerializeComponent(ISerializer*& serialized, rapidjson::Value& jsonValue)
{
	for (std::pair<const char*, std::string*>&& str : serialized->m_StringProperties)
	{
		rapidjson::Value stringKey(str.first, m_Document.GetAllocator());
		rapidjson::Value stringValue(str.second->c_str(), m_Document.GetAllocator());
		jsonValue.AddMember(stringKey, stringValue, m_Document.GetAllocator());
	}

	// Vec3 Properties
	for (std::pair<const char*, glm::vec3*>&& vec3 : serialized->m_Vec3Properties)
	{
		rapidjson::Value vec3Key(vec3.first, m_Document.GetAllocator());
		rapidjson::Value vec3Value(rapidjson::kArrayType);
		vec3Value.PushBack(vec3.second->x, m_Document.GetAllocator());
		vec3Value.PushBack(vec3.second->y, m_Document.GetAllocator());
		vec3Value.PushBack(vec3.second->z, m_Document.GetAllocator());
		jsonValue.AddMember(vec3Key, vec3Value, m_Document.GetAllocator());
	}

	return jsonValue;
}

