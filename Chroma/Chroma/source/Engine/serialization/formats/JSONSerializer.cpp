#include "JSONSerializer.h"


namespace Chroma
{
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


	JSONSerializer::JSONSerializer()
	{
	}


	JSONSerializer::~JSONSerializer()
	{
	}
}
