#include "Screen.h"
#include <render/Render.h>
#ifdef EDITOR
#include <editor/ui/EditorUI.h>
#endif

namespace Chroma
{
	GLFWwindow* Screen::m_Window;
	int Screen::m_Height;
	int Screen::m_Width;
	int Screen::last_width;
	int Screen::last_height;

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
		glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
		CHROMA_INFO("Window Successfuly Initialized.");
	}


	void Screen::SetDimensions(int const& newWidth, int const& newHeight)
	{
		// update renderer
		Chroma::Render::ResizeBuffers(newWidth, newHeight);

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

#ifdef EDITOR
	std::pair<int, int> Screen::GetWidthHeight()
	{
		std::pair<int, int> viewportDimensions =  Chroma::EditorUI::GetViewportDimensions();
		return std::make_pair(viewportDimensions.first, viewportDimensions.second);
	}
#else
	std::pair<int, int> Screen::GetWidthHeight()
	{
		glfwGetWindowSize(m_Window, &m_Width, &m_Height);
		return std::make_pair(m_Width, m_Height);
	}
#endif

	// glfw callbacks
	// --------------------
	void Screen::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{

		if (width == 0 || height == 0)
		{
			return;
		}

#ifdef EDITOR
		Chroma::EditorUI::ResizeEditorUI(width, height);
#else
		SetDimensions(width, height);	
#endif

	}
}
