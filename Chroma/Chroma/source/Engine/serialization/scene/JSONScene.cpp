#include "JSONScene.h"

void JSONScene::AddNewEntity(ISerializer*& serialized)
{
	rapidjson::Value newEntity(rapidjson::kObjectType);
	// Get Type Name
	std::string entityType = Chroma::Type::GetName(serialized->m_EntityType);
	rapidjson::Value entityTypeName(entityType.c_str(), m_Document.GetAllocator());
	// Get UID
	std::string uidKey = serialized->m_UID.data;

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

