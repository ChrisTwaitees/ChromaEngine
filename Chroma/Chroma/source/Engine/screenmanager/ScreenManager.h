#ifndef _CHROMA_SCREENMANAGER_H_
#define _CHROMA_SCREENMANAGER_H_
// stl
#include <vector>
#include <iostream>
// glad and glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// glm
#include <glm/glm.hpp>
// GUI
#include <gui/GUI.h>
// camera
#include <camera/Camera.h>
//config
#include <ChromaConfig.h>
// framebuffer
#include <buffer/Framebuffer.h>
#include <core/Core.h>
// time 
#include <time/Time.h>

namespace Chroma
{
	class ScreenManager
	{
		// window
		static GLFWwindow* m_Window;

		// setup
		static bool ConfigureWindow();

		// renderer
		static void UpdateDimensions(int width, int height);

		// callbacks
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	public:
		static void Init();
		// getters and setters
		inline static GLFWwindow*& GetWindow() { return m_Window; };
		static std::pair<int, int> GetWidthHeight();





		// status
		static int GetShouldClose() { return glfwWindowShouldClose(m_Window); };

		// functions
		static void StartLoop();
		static void EndLoop();
		static void Close();



		// construction
		ScreenManager();
		~ScreenManager();
	};
}



#endif
