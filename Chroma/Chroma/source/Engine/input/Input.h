#ifndef _CHROMA_INPUT_
#define _CHROMA_INPUT_


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

class Input
{
	// components
	GLFWwindow* m_Window{ nullptr };
	Camera* m_Camera{ nullptr };

	// attrs
	bool m_CursorEnabled{ true };
	static double m_CaptureMouseX, m_CaptureMouseY;
	double m_MouseX{ 0.0f }, m_MouseY{ 0.0f };
	double m_LastMouseX{ 0.0f }, m_LastMouseY{ 0.0f };
	glm::vec2 m_MouseXYOffset{ glm::vec2(0.0f, 0.0f) };
	float m_PickedMouseX{ 0.0f }, m_PickedMouseY{ 0.0f };

	glm::vec3 m_LastMouseRay{ glm::vec3(0) };
	double m_DeltaTime{ 0.0f };
	int m_ScreenWidth{ 0 }, m_ScreenHeight{ 0 };

	// functions 
	void Initialize();
	void UpdateMouseCoordinates();
	void UpdateMousePicker();
	glm::vec3 ScreenToWorldRay(float const& mouseX, float const& mouseY);

	// callbacks
	static void mouse_aim_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);


public:
	enum Key {LEFT_SHIFT, RIGHT_SHIFT, LEFT_ALT, RIGHT_ALT, SPACEBAR, ESCAPE, A, C, D, E, Q, S, W, Y, P,
	LEFT_MOUSE, LEFT_CTRL, RIGHT_CTRL, RIGHT_MOUSE, MIDDLE_MOUSE, LEFT_MOUSE_RELEASE, RIGHT_MOUSE_RELEASE, MIDDLE_MOUSE_RELEASE};
	bool IsPressed(Key KeySelection);

	//  functions
	void Process();
	void BindMousePickerCallback(std::function<void()> callback);
	std::function<void()> m_MousePickerCallback;

	// getters and setters
	// mouse x
	inline double GetMouseX() const { return m_MouseX; };
	inline double GetMouseXOffset() const { return m_MouseXYOffset.x; };
	// mouse y
	inline double GetMouseY() const { return m_MouseY; };
	inline double GetMouseYOffset() const { return m_MouseXYOffset.y; };
	// mouse xy
	inline glm::vec2 GetMouseXY() const { return glm::vec2(m_MouseX, m_MouseY); };
	inline glm::vec2 GetMouseXYOffset() const { return m_MouseXYOffset; };
	// cursor attrs
	inline bool GetCursorEnabled() const { return m_CursorEnabled; };
	inline glm::vec3 GetLastRay() const { return m_LastMouseRay; };

	// bind
	void BindWindow(GLFWwindow* windowVal);
	void BindCamera(Camera* cam) { m_Camera = cam; };

	// deltaTime
	void SetDeltaTime(double deltaTimeVal) { m_DeltaTime = deltaTimeVal; };
	double GetDeltaTime() {	return m_DeltaTime; };

	// constructors
	Input() {};
	Input(GLFWwindow* windowVal);
	~Input();
};

#endif