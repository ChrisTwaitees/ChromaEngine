#include "Core.h"

namespace Chroma
{
	void Core::Init()
	{
		// Logging
		Chroma::Log::Init();
		CHROMA_INFO("Chroma Initializing...");

		// OpenGL 3
		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			CHROMA_FATAL("Failed to Initialize GLAD");
		}
	}

}