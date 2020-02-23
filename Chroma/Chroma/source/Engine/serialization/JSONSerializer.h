#ifndef _CHROMA_JSON_SERIALIZER_H_
#define _CHROMA_JSON_SERIALIZER_H_

#include <serialization/formats/Json.h>
#include <serialization/ISerializer.h>

class JSONSerializer : public ISerializer
{
public:
	void StartObject(const char* objectName, UID const& uid) override;

	const char* ToString() override;

	JSONSerializer();
	~JSONSerializer();
};

#endif
