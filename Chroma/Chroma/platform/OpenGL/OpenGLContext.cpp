#include "OpenGLContext.h"

#include <glfw/glfw3.h>
#include <glad/glad.h>

namespace Chroma
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CHROMA_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		// set glfw window to opengl context
		glfwMakeContextCurrent(m_WindowHandle);

		// load opengl functions
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CHROMA_ASSERT(status, "Failed to initialize Glad!");

		// info
		CHROMA_INFO("OpenGL Info:");
		CHROMA_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		CHROMA_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		CHROMA_INFO("  Version: {0}", glGetString(GL_VERSION));

		// check version
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		CHROMA_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Hazel requires at least OpenGL version 4.5!");

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}