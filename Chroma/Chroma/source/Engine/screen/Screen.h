#ifndef _CHROMA_SCREENMANAGER_H_
#define _CHROMA_SCREENMANAGER_H_

// glad and glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// core
#include <core/Core.h>

namespace Chroma
{
	class Screen
	{
		// window
		static GLFWwindow* m_Window;

		// dimensions
		static int m_Width, m_Height;

		// callbacks
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	public:
		static void Init();

		// getters and setters
		inline static GLFWwindow*& GetWindow() { return m_Window; };
		static std::pair<int, int> GetWidthHeight();

		static void SetDimensions(int const& newWidth, int const& newHeight);
		// status
		static int GetShouldClose() { return glfwWindowShouldClose(m_Window); };

		// functions
		static void Update();
		static void Close();
	};
}



#endif
