#include "JSONScene.h"
#include <scene/Scene.h>

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
	if (itr == GetComponents().MemberEnd())
	{
		rapidjson::Value newEntityTypeObject(rapidjson::kObjectType);
		GetEntities().AddMember(entityTypeName, newEntityTypeObject, m_Document.GetAllocator());
	}

	GetEntities()[entityType.c_str()].AddMember(rapidjson::StringRef(uidKey.c_str()), newEntity, m_Document.GetAllocator());
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
	if (itr == GetComponents().MemberEnd())
	{
		rapidjson::Value newComponentTypeObject(rapidjson::kObjectType);
		GetComponents().AddMember(componentTypeName, newComponentTypeObject, m_Document.GetAllocator());
	}

	GetComponents()[componentType.c_str()].AddMember(rapidjson::StringRef(uidKey.c_str()), newComponent, m_Document.GetAllocator());

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
		jsonValue[componentType.c_str()].PushBack(rapidjson::StringRef(componentUID.data.c_str()), m_Document.GetAllocator());
	}

}

void JSONScene::SerializeComponent(ISerializer*& serialized, rapidjson::Value& jsonValue)
{
	// Serialized generic types
	SerializeTypes(serialized, jsonValue);
}

void JSONScene::SerializeTypes(ISerializer*& serialized, rapidjson::Value& jsonValue)
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

	// Vec4 Properties
	for (std::pair<const char*, glm::vec4*>&& vec4 : serialized->m_Vec4Properties)
	{
		rapidjson::Value vec4Key(vec4.first, m_Document.GetAllocator());
		rapidjson::Value vec4Value(rapidjson::kArrayType);
		vec4Value.PushBack(vec4.second->x, m_Document.GetAllocator());
		vec4Value.PushBack(vec4.second->y, m_Document.GetAllocator());
		vec4Value.PushBack(vec4.second->z, m_Document.GetAllocator());
		vec4Value.PushBack(vec4.second->w, m_Document.GetAllocator());
		jsonValue.AddMember(vec4Key, vec4Value, m_Document.GetAllocator());
	}

	// Quat Properties
	for (std::pair<const char*, glm::quat*>&& quat : serialized->m_QuatProperties)
	{
		rapidjson::Value quatKey(quat.first, m_Document.GetAllocator());
		rapidjson::Value quatValue(rapidjson::kArrayType);
		quatValue.PushBack(quat.second->x, m_Document.GetAllocator());
		quatValue.PushBack(quat.second->y, m_Document.GetAllocator());
		quatValue.PushBack(quat.second->z, m_Document.GetAllocator());
		quatValue.PushBack(quat.second->w, m_Document.GetAllocator());
		jsonValue.AddMember(quatKey, quatValue, m_Document.GetAllocator());
	}


}



