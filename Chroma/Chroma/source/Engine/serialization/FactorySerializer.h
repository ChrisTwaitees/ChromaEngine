#ifndef _CHROMA_FACTORYSERIALIZER_H_
#define _CHROMA_FACTORYSERIALIZER_H_

#include <types/Types.h>

namespace Chroma
{
	class ISerializer;
	class IDeserializer;

	class FactorySerializer
	{
	public:
		static ISerializer* GetSerializer(Chroma::Type::Serialization serializationType);
		static IDeserializer* GetDeserializer(Chroma::Type::Serialization serializationType);
	};

}
#endif