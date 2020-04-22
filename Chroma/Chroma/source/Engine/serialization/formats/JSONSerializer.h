#ifndef _CHROMA_JSON_SERIALIZER_H_
#define _CHROMA_JSON_SERIALIZER_H_

#include <serialization/formats/Json.h>
#include <serialization/ISerializer.h>


class JSONSerializer : public ISerializer
{
public:
	virtual void StartObject(const Chroma::Type::Component& componentType, const UID& uid) override;
	virtual void StartObject(const Chroma::Type::Entity& entityType, const UID& uid) override;

	JSONSerializer();
	~JSONSerializer();
};

#endif
