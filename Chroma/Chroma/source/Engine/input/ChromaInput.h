#ifndef _CHROMA_INPUT_HANDLER_
#define _CHROMA_INPUT_HANDLER_


#include <iostream>

// glfw
#include <GLFW/glfw3.h>
// glm
#include <glm/glm.hpp>

#include "screenmanager/ChromaScreenManagerConfig.h"


class ChromaInput
{
	GLFWwindow* window;
	static double CaptureMouseX, CaptureMouseY;
	double MouseX, MouseY;
	bool cursorEnabled{ true };
	double deltaTime;

	// functions 
	void initialize();

	// callbacks
	static void mouse_aim_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void updateMouseCoordinates();

public:
	enum Key {LEFT_SHIFT, RIGHT_SHIFT, SPACEBAR, A, C, D, E, Q, S, W};
	bool isPressed(Key KeySelection);

	//  functions
	void process();

	// getters and setters
	double getMouseX() { return MouseX; };
	double getMouseY() { return MouseY; };
	glm::vec2 getMouseXY() { return glm::vec2(MouseX, MouseY); };
	bool getCursorEnabled() { return cursorEnabled; };

	// bind
	void bindWindow(GLFWwindow* windowVal);

	// deltaTime
	void setDeltaTime(double deltaTimeVal) { deltaTime = deltaTimeVal; };
	double getDeltaTime() {	return deltaTime; };
	// constructors
	ChromaInput();
	ChromaInput(GLFWwindow* windowVal);
	~ChromaInput();
};

#endif