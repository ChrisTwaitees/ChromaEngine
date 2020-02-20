#include "JSONSerializer.h"



void JSONSerializer::StartObject(const char* objectName, const char* objectID)
{
	CHROMA_TRACE("STARTING OBJECT : {0} with UID : {1}", objectName, objectID);
}

void JSONSerializer::AddProperty(const char* key, const char* value)
{
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
