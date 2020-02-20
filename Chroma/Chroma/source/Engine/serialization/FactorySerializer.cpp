#include "FactorySerializer.h"



ISerializer* FactorySerializer::GetSerializer(Serialization::FORMAT format)
{
	switch (format)
	{
	case(Serialization::FORMAT::JSON):
		{
			return new JSONSerializer;
			break;
		}
	}
}
