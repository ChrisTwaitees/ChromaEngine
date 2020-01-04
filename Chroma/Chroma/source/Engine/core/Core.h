#ifndef _CHROMA_CORE_
#define _CHROMA_CORE_

// Config
#include <ChromaConfig.h>

// Logging
#include <logging/Log.h>

// Screen
#include <screen/Screen.h>

// Glad
#include <glad/glad.h>


namespace Chroma
{
	struct Core
	{
		static void Init();
	};
}

#endif