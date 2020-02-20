#ifndef _CHROMA_JSON_SERIALIZER_H_
#define _CHROMA_JSON_SERIALIZER_H_

#include <serialization/formats/Json.h>
#include <serialization/ISerializer.h>

class JSONSerializer : public ISerializer
{
public:
	void StartObject(const char* objectName, const char* objectID) override;
	void AddProperty(const char* key, const char* value) override;
	const char* ToString() override;

	JSONSerializer();
	~JSONSerializer();
};

#endif