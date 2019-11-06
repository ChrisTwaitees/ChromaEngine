#ifndef _CHROMA_INPUT_HANDLER_
#define _CHROMA_INPUT_HANDLER_


#include <iostream>

// glfw
#include <GLFW/glfw3.h>
// glm
#include <glm/glm.hpp>

#include "screenmanager/ChromaScreenManagerConfig.h"
#include "camera/Camera.h"
#include "input/MousePicker.h"

class Camera;
class MousePicker;

class ChromaInput
{
	// components
	GLFWwindow* mWindow;
	Camera* mCamera;
	MousePicker mMousePicker;

	// attrs
	static double CaptureMouseX, CaptureMouseY;
	double MouseX{ 0.0f }, MouseY{ 0.0f };
	float PickedMouseX{ 0.0f }, PickedMouseY{ 0.0f };
	bool cursorEnabled{ true };
	double deltaTime;

	// functions 
	void initialize();

	// callbacks
	static void mouse_aim_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
	void updateMouseCoordinates();
	void updateMousePicker();

public:
	enum Key {LEFT_SHIFT, RIGHT_SHIFT, LEFT_ALT, RIGHT_ALT, SPACEBAR, ESCAPE, A, C, D, E, Q, S, W,
	LEFT_MOUSE, RIGHT_MOUSE, MIDDLE_MOUSE, LEFT_MOUSE_RELEASE, RIGHT_MOUSE_RELEASE, MIDDLE_MOUSE_RELEASE};
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
	void bindCamera(Camera* cam) { mCamera = cam; };

	// deltaTime
	void setDeltaTime(double deltaTimeVal) { deltaTime = deltaTimeVal; };
	double getDeltaTime() {	return deltaTime; };
	// constructors
	ChromaInput() {};
	ChromaInput(GLFWwindow* windowVal);
	~ChromaInput();
};

#endif