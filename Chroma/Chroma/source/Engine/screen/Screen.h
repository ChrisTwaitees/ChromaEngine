#ifndef _CHROMA_SCREENMANAGER_H_
#define _CHROMA_SCREENMANAGER_H_

//common
#include <common/PrecompiledHeader.h>
// core
#include <common/CoreCommon.h>

namespace Chroma
{
	class Screen
	{
	public:
		static void Init();

		// getters and setters
		inline static GLFWwindow*& GetWindow() { return m_Window; };
		static std::pair<int, int> GetWidthHeight();

		static void SetDimensions(int const& newWidth, int const& newHeight);
		// status
		inline static bool IsRunning() { return !glfwWindowShouldClose(m_Window); }

		// functions
		static void Update();
		static void Close();

	private:
		// window
		static GLFWwindow* m_Window;

		// dimensions
		static int m_Width, m_Height;

		// callbacks
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

		// stats
		static int last_width;
		static int last_height;
		static std::string m_WindowName;
		
	};
}



#endif
