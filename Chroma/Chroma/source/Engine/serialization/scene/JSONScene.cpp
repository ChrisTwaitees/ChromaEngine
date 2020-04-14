#include "JSONScene.h"

void JSONScene::AddNewEntity(ISerializer*& serialized)
{
	rapidjson::Value newEntity(rapidjson::kObjectType);

	m_Document[CHROMA_ROOT][CHROMA_LEVEL][CHROMA_ENTITIES][Chroma::Type::GetName(serialized->m_EntityType).c_str()].AddMember("TestEntity", newEntity, m_Document.GetAllocator());
}

void JSONScene::AddNewComponent(ISerializer*& serialized)
{
	rapidjson::Value newComponent(rapidjson::kObjectType);

	m_Document[CHROMA_ROOT][CHROMA_LEVEL][CHROMA_COMPONENTS][Chroma::Type::GetName(serialized->m_ComponentType).c_str()].AddMember("TestComponent", newComponent, m_Document.GetAllocator());

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

