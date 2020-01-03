#ifndef _CHROMA_CORE_
#define _CHROMA_CORE_

// all core includes for Chroma

// Config
#include <ChromaConfig.h>

// Logging
#include <logging/Log.h>

// Time
#include <time/Time.h>

// Input
#include <input/Input.h>

// Screen
#include <screen/Screen.h>

// Workers
#include <worker/IWorker.h>

// Glad
#include <glad/glad.h>


namespace Chroma
{
	struct Core
	{
		static void InitLibs();
	};
}

#endif