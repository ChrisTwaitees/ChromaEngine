#ifndef _CHOMA_UID_H_
#define _CHOMA_UID_H_

// generates unique hexadecimal ID
#include <sstream>
#include <random>
#include <string>
#include <ChromaConfig.h>

namespace UID
{
	const char* GenerateNewUID();
}


#endif