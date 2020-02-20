#ifndef _CHROMA_SERIALIZER_H_
#define _CHROMA_SERIALIZER_H_

// Factory method read from : https://realpython.com/factory-method-python/

struct Serialization
{
	enum FORMAT{JSON};
};


class ISerializer
{
public:
	virtual void StartObject(const char* objectName, const char* objectID) = 0;
	virtual void AddProperty(const char* key, const char* value) = 0;
	virtual const char* ToString() = 0;

	ISerializer() {};
	virtual ~ISerializer() {};
};

#endif 
