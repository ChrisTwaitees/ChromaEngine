#include "JSONSerializer.h"


void JSONSerializer::StartObject(const Chroma::Type::Component& componentType, const UID& uid)
{
	m_ComponentType = componentType;
	m_UID = uid;
}

void JSONSerializer::StartObject(const Chroma::Type::Entity& entityType, const UID& uid)
{
	m_EntityType = entityType;
	m_UID = uid;
}

const char* JSONSerializer::ToString()
{
	return nullptr;
}

JSONSerializer::JSONSerializer()
{
}


JSONSerializer::~JSONSerializer()
{
}
