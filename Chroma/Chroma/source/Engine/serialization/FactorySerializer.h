#ifndef _CHROMA_FACTORYSERIALIZER_H_
#define _CHROMA_FACTORYSERIALIZER_H_

#include <serialization/JSONSerializer.h>

class FactorySerializer
{
public:
	static ISerializer* GetSerializer(Serialization::FORMAT format);
};

#endif