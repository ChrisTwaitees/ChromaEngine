#include "JSONSerializer.h"



void JSONSerializer::StartObject(const char* objectName, UID const& uid)
{
	CHROMA_TRACE("STARTING OBJECT : {0} with UID : {1}", objectName, uid.data);
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
