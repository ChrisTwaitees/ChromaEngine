#ifndef CHROMA_DESERIALIZER_H
#define CHROMA_DESERIALIZER_H

// Factory method read from : https://realpython.com/factory-method-python/
#include <common/PrecompiledHeader.h>
#include <types/Types.h> 
#include <uid/UID.h>


class IDeserializer
{
public:
	IDeserializer() {};
	~IDeserializer() {};
};

#endif // CHROMA_DESERIALIZER_H