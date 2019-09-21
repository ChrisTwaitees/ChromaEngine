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
	int counter;
	void initialize();
public:
	void draw();
	void attachWindow(GLFWwindow& window_val);


	ChromaGUI();
	ChromaGUI(GLFWwindow* window_val);
	~ChromaGUI();
};

#endif