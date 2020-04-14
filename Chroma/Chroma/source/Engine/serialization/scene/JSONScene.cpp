#include "JSONScene.h"

void JSONScene::AddNewEntity(const UID& uid)
{
	rapidjson::Value newEntity(rapidjson::kObjectType);
	std::string uidKey(uid.data);

	m_Document[CHROMA_ROOT][CHROMA_LEVEL][CHROMA_ENTITIES].AddMember("TestEntity", newEntity, m_Document.GetAllocator());
}

void JSONScene::AddNewComponent(const UID& uid)
{
	rapidjson::Value newComponent(rapidjson::kObjectType);
	std::string uidKey(uid.data);

	m_Document[CHROMA_ROOT][CHROMA_LEVEL][CHROMA_COMPONENTS].AddMember("TestComponent", newComponent, m_Document.GetAllocator());

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

