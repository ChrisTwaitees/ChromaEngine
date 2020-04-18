#ifndef _CHROMA_FACTORYSERIALIZER_H_
#define _CHROMA_FACTORYSERIALIZER_H_

class ISerializer;
class IDeserializer;

#include <types/Types.h>

class FactorySerializer
{
public:
	static ISerializer* GetSerializer(Chroma::Type::Serialization serializationType);
	static IDeserializer* GetDeserializer(Chroma::Type::Serialization serializationType);
};

#endif