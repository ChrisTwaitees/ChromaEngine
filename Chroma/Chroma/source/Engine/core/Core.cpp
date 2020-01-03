#include "Core.h"

namespace Chroma
{
	void Core::Init()
	{
		// Logging
		Chroma::Log::Init();
		CHROMA_INFO("Chroma Core Initializing...");

		// Screen / GLFW
		Chroma::Screen::Init();
		CHROMA_INFO("Chroma Screen Initialized.");

		// GLAD
		// OpenGL 3
		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			CHROMA_FATAL("Failed to Initialize GLAD");
		}
		else
		{

			CHROMA_INFO("Chroma OpenGL Initialized.");
		}

		// GUI
		Chroma::GUI::Init();
		CHROMA_INFO("Chroma GUI Initialized.");

	}

}