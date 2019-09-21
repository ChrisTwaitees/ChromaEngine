#ifndef _CHROMA_GUI_
#define _CHROMA_GUI_
// imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//stl
#include <iostream>
class ChromaGUI
{
protected:
	GLFWwindow* window;
	void initialize();
public:
	void Start();
	void End();
	void attachWindow(GLFWwindow& window_val);


	ChromaGUI();
	ChromaGUI(GLFWwindow* window_val);
	~ChromaGUI();
};

#endif