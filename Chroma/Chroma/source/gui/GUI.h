#ifndef _CHROMA_GUI_
#define _CHROMA_GUI_
// imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
class GUI
{
protected:
	GLFWwindow* window;
	int counter;
	void initialize();
public:
	void draw();
	GUI(GLFWwindow* window_val);
	~GUI();
};

#endif