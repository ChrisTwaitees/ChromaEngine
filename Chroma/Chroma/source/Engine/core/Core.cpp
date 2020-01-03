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
			// Enabling Render Features
			// ---------------------------------------
			// Enable depth buffer
			glEnable(GL_DEPTH_TEST);
			// Enable Face Culling
			glEnable(GL_CULL_FACE);
			// Enabling MSAA
			glEnable(GL_MULTISAMPLE);
			// Setting Clear Color
			glClearColor(SCREEN_DEFAULT_COLOR.x, SCREEN_DEFAULT_COLOR.y, SCREEN_DEFAULT_COLOR.z, SCREEN_DEFAULT_COLOR.w);
			CHROMA_INFO("Chroma OpenGL Initialized.");
		}

		// GUI
		Chroma::GUI::Init();
		CHROMA_INFO("Chroma GUI Initialized.");

	}

}