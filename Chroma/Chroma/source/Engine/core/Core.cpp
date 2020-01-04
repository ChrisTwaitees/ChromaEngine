#include "Core.h"

namespace Chroma
{
	void Core::Init()
	{
		// Screen / GLFW
		Chroma::Screen::Init();

		// GLAD OpenGL 3
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			CHROMA_FATAL("OpenGL Functions Failed to Bind, Check your implementation of GLAD.");
		else
			CHROMA_INFO("OpenGL Functions Bound.");
	}

}