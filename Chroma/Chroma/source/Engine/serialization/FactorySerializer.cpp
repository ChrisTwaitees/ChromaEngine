#include "FactorySerializer.h"
#include <serialization/formats/JSONSerializer.h>
#include <serialization/formats/JSONDeserializer.h>



ISerializer* FactorySerializer::GetSerializer(Chroma::Type::Serialization serializationType)
{
	switch (serializationType)
	{
	case(Chroma::Type::Serialization::kJSON):
	{
		return new JSONSerializer;
		break;
	}
	}
}

IDeserializer* FactorySerializer::GetDeSerializer(Chroma::Type::Serialization serializationType)
{
	switch (serializationType)
	{
	case(Chroma::Type::Serialization::kJSON):
	{
		return new JSONDeserializer;
		break;
	}
	}
}
