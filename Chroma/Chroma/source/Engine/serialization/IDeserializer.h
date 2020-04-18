#ifndef CHROMA_DESERIALIZER_H
#define CHROMA_DESERIALIZER_H

// Factory method read from : https://realpython.com/factory-method-python/
#include <common/PrecompiledHeader.h>
#include <types/Types.h> 
#include <uid/UID.h>

#include <entity/Entity.h>
#include <model/StaticMesh.h>
#include <model/SkinnedMesh.h>


class IDeserializer
{
public:
	template <class objectType, typename ChromaType> 
	objectType CreateObject(ChromaType type, std::string data)
	{
		CHROMA_WARN("Could not create Object Based on Deserialization Data!");
	}

	IDeserializer() {};
	~IDeserializer() {};
};


#ifdef DEBUG
#define DESERIALIZE_START CHROMA_INFO("Deserialization of Object Type : {} Started.", Chroma::Type::GetName(type));

#else
#define DESERIALIZE_START
#endif


#endif // CHROMA_DESERIALIZER_H