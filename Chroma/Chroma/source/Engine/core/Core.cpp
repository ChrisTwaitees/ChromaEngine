#include "Core.h"

namespace Chroma
{
	void Core::InitLibs()
	{
		// OpenGL 3
		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			CHROMA_FATAL("Failed to Initialize GLAD");
		}
	}

}