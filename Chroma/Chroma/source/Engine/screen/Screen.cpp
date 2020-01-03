#include "Screen.h"

namespace Chroma
{
	GLFWwindow* Screen::m_Window;
	int Screen::m_Height;
	int Screen::m_Width;

	void Screen::Init()
	{
		// Configure Window
		// glfw: initialize and configure
		// ------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, MSAA_SAMPLES);

		// glfw window creation
		// --------------------
		m_Window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CHROMA", NULL, NULL);
		if (m_Window == NULL)
		{
			CHROMA_FATAL("Failed to Initialize Chroma window");
			glfwTerminate();
		}
		glfwMakeContextCurrent(m_Window);
	}


	void Screen::SetDimensions(int const& newWidth, int const& newHeight)
	{
		// make sure the viewport matches the new window dimensions;  
		glViewport(0, 0, newWidth, newHeight);
	}


	void Screen::Update()
	{
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	void Screen::Close()
	{
		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
	}

	std::pair<int, int> Screen::GetWidthHeight()
	{
		glfwGetWindowSize(m_Window, &m_Width, &m_Height);
		return std::make_pair(m_Width, m_Height);
	}


	// glfw callbacks
	// --------------------
	void Screen::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		SetDimensions(width, height);
	}
}
