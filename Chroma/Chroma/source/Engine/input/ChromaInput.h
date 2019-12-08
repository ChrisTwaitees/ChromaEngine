#ifndef _CHROMA_INPUT_HANDLER_
#define _CHROMA_INPUT_HANDLER_


#include <iostream>
#include <functional>

// glfw
#include <GLFW/glfw3.h>
// glm
#include <glm/glm.hpp>
// glad
#include <glad/glad.h>
#include <camera/Camera.h>

class Camera;

class ChromaInput
{
	bool wireframe;
	// components
	GLFWwindow* mWindow;
	Camera* mCamera;

	// attrs
	static double CaptureMouseX, CaptureMouseY;
	double MouseX{ 0.0f }, MouseY{ 0.0f };
	double lastMouseX{ 0.0f }, lastMouseY{ 0.0f };
	glm::vec2 MouseXYOffset{ glm::vec2(0.0f, 0.0f) };
	float PickedMouseX{ 0.0f }, PickedMouseY{ 0.0f };
	glm::vec3 lastMouseRay{ glm::vec3(0) };
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
	glm::vec3 screenToWorldRay(float const& mouseX, float const& mouseY);

	// attrs
	int screenWidth, screenHeight;

public:
	enum Key {LEFT_SHIFT, RIGHT_SHIFT, LEFT_ALT, RIGHT_ALT, SPACEBAR, ESCAPE, A, C, D, E, Q, S, W, Y, P,
	LEFT_MOUSE, LEFT_CTRL, RIGHT_CTRL, RIGHT_MOUSE, MIDDLE_MOUSE, LEFT_MOUSE_RELEASE, RIGHT_MOUSE_RELEASE, MIDDLE_MOUSE_RELEASE};
	bool isPressed(Key KeySelection);

	//  functions
	void process();
	void bindMousePickerCallback(std::function<void()> callback);
	std::function<void()> mMousePickerCallback;

	// getters and setters
	// mouse x
	inline double getMouseX() const { return MouseX; };
	inline double getMouseXOffset() const { return MouseXYOffset.x; };
	// mouse y
	inline double getMouseY() const { return MouseY; };
	inline double getMouseYOffset() const { return MouseXYOffset.y; };
	// mouse xy
	inline glm::vec2 getMouseXY() const { return glm::vec2(MouseX, MouseY); };
	inline glm::vec2 getMouseXYOffset() const { return MouseXYOffset; };
	// cursor attrs
	inline bool getCursorEnabled() const { return cursorEnabled; };
	inline glm::vec3 getLastRay() const { return lastMouseRay; };

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