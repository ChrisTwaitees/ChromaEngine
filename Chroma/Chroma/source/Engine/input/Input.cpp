#include "Input.h"


namespace Chroma
{
	// Window and Camera
	GLFWwindow* Input::m_Window;
	Camera* Input::m_Camera;

	// Controller
	bool Input::m_ControllerEnabled;

	int Input::m_ControllerAxesCount;
	int Input::m_ControllerButtonsCount;
	const unsigned char* Input::m_ControllerButtonsMapping;

	float Input::m_ControllerRightVertical;
	float Input::m_ControllerRightHorizontal;

	float Input::m_ControllerLeftVertical;
	float Input::m_ControllerLeftHorizontal;

	float Input::m_ControllerLeftBumper;
	float Input::m_ControllerRightBumper;

	// Mouse
	bool   Input::m_CursorEnabled;

	double Input::m_MouseX;
	double Input::m_CaptureMouseX;
	double Input::m_CaptureMouseY;
	double Input::m_MouseY;
	double Input::m_LastMouseX;
	double Input::m_LastMouseY;

	float Input::m_PickedMouseX;
	float Input::m_PickedMouseY;
	glm::vec2 Input::m_MouseXYOffset;
	glm::vec3 Input::m_LastMouseRay;

	int Input::m_ScreenWidth;
	int Input::m_ScreenHeight;
	std::function<void()> Input::m_MousePickerCallback;

	void Input::Initialize()
	{
		// glfw attach callbacks
		glfwSetCursorPosCallback(m_Window, mouse_aim_callback);
		glfwSetScrollCallback(m_Window, mouse_scroll_callback);
		glfwSetMouseButtonCallback(m_Window, mouse_click_callback);
	}

	void Input::BindMousePickerCallback(std::function<void()> callback)
	{
		m_MousePickerCallback = callback;
	}


	bool Input::IsPressed(Key KeySelection)
	{
		switch (KeySelection) {
		case LEFT_SHIFT:
			return glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
			break;
		case RIGHT_SHIFT:
			return glfwGetKey(m_Window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
			break;
		case LEFT_ALT:
			return glfwGetKey(m_Window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
			break;
		case RIGHT_ALT:
			return glfwGetKey(m_Window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS;
			break;
		case SPACEBAR:
			return glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS;
			break;
		case ESCAPE:
			return glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
			break;
		case LEFT_CTRL:
			return glfwGetKey(m_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
			break;
		case RIGHT_CTRL:
			return glfwGetKey(m_Window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
			break;
		case A:
			return glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS;
			break;
		case C:
			return glfwGetKey(m_Window, GLFW_KEY_C) == GLFW_PRESS;
			break;
		case D:
			return glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS;
			break;
		case E:
			return glfwGetKey(m_Window, GLFW_KEY_E) == GLFW_PRESS;
			break;
		case P:
			return glfwGetKey(m_Window, GLFW_KEY_P) == GLFW_PRESS;
			break;
		case Q:
			return glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS;
			break;
		case S:
			return glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS;
			break;
		case Y:
			return glfwGetKey(m_Window, GLFW_KEY_Y) == GLFW_PRESS;
			break;
		case W:
			return glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS;
			break;
		case LEFT_MOUSE:
			return glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
			break;
		case RIGHT_MOUSE:
			return glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS;
			break;
		case MIDDLE_MOUSE:
			return glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS;
			break;
		case LEFT_MOUSE_RELEASE:
			return glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE;
			break;
		case RIGHT_MOUSE_RELEASE:
			return glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE;
			break;
		case MIDDLE_MOUSE_RELEASE:
			return glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_3) == GLFW_RELEASE;
			break;
		case CROSS:
			return m_ControllerButtonsMapping[CROSS_MAPPING] == GLFW_RELEASE;
			break;
		case SQUARE:
			return m_ControllerButtonsMapping[SQUARE_MAPPING] == GLFW_RELEASE;
			break;
		case CIRCLE:
			return m_ControllerButtonsMapping[CIRCLE_MAPPING] == GLFW_RELEASE;
			break;
		case TRIANGLE:
			return m_ControllerButtonsMapping[TRIANGLE_MAPPING] == GLFW_RELEASE;
			break;
		case R1:
			return m_ControllerButtonsMapping[R1_MAPPING] == GLFW_RELEASE;
			break;
		case L1:
			return m_ControllerButtonsMapping[L1_MAPPING] == GLFW_RELEASE;
			break;
		case R2:
			return m_ControllerButtonsMapping[R2_MAPPING] == GLFW_RELEASE;
			break;
		case L2:
			return m_ControllerButtonsMapping[L2_MAPPING] == GLFW_RELEASE;
			break;
		case DPADLEFT:
			return m_ControllerButtonsMapping[DPADLEFT_MAPPING] == GLFW_RELEASE;
			break;
		case DPADRIGHT:
			return m_ControllerButtonsMapping[DPADRIGHT_MAPPING] == GLFW_RELEASE;
			break;
		case DPADUP:
			return m_ControllerButtonsMapping[DPADUP_MAPPING] == GLFW_RELEASE;
			break;
		case DPADDOWN:
			return m_ControllerButtonsMapping[DPADDOWN_MAPPING] == GLFW_RELEASE;
			break;
		case OPTIONS:
			return m_ControllerButtonsMapping[OPTIONS_MAPPING] == GLFW_RELEASE;
			break;
		case SHARE:
			return m_ControllerButtonsMapping[SHARE_MAPPING] == GLFW_RELEASE;
			break;
		case TOUCHPAD:
			return m_ControllerButtonsMapping[TOUCHPAD_MAPPING] == GLFW_RELEASE;
			break;
		}

		return false;
	}

	void Input::Process()
	{
		// should close?
		if (IsPressed(ESCAPE)) {
			glfwSetWindowShouldClose(m_Window, true);
			return;
		}

		// capture / release mouse
		int cursorMode = glfwGetInputMode(m_Window, GLFW_CURSOR);
		if (IsPressed(LEFT_CTRL))
		{
			if (cursorMode == GLFW_CURSOR_DISABLED)
				glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else
				glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		m_CursorEnabled = cursorMode == GLFW_CURSOR_DISABLED;

		// MOUSE
		if (m_CursorEnabled)
		{
		}
		UpdateMouseCoordinates();
		// mouse picker
		UpdateMousePicker();

		// CONTROLLER
		m_ControllerEnabled = (glfwJoystickPresent(GLFW_JOYSTICK_1) > 0) ? true : false;
		UpdateController();
	}

	void Input::BindWindow(GLFWwindow* windowVal)
	{
		m_Window = windowVal;
		Initialize();
	}

	// glfw callbacks
	void Input::mouse_aim_callback(GLFWwindow* window, double xpos, double ypos)
	{
		m_CaptureMouseX = xpos;
		m_CaptureMouseY = ypos;

	}

	void Input::mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		return;
	}

	void Input::mouse_click_callback(GLFWwindow* window, int button, int action, int mods)
	{
	}

	void Input::UpdateMouseCoordinates()
	{
		// current coordinates
		m_MouseX = m_CaptureMouseX;
		m_MouseY = m_CaptureMouseY;

		// offset
		m_MouseXYOffset.x = m_MouseX - m_LastMouseX;
		m_MouseXYOffset.y = m_LastMouseY - m_MouseY;

		// last pos
		m_LastMouseX = m_MouseX;
		m_LastMouseY = m_MouseY;
	}

	void Input::UpdateMousePicker()
	{
		if (IsPressed(LEFT_MOUSE))
		{
			m_PickedMouseX = m_MouseX;
			m_PickedMouseY = m_MouseY;
			m_LastMouseRay = ScreenToWorldRay(m_PickedMouseX, m_PickedMouseY);
			m_MousePickerCallback();
		}
	}

	void Input::UpdateController()
	{
		if (m_ControllerEnabled)
		{
			// Axes
			const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &m_ControllerAxesCount);
			for (int i = 0; i < m_ControllerAxesCount; i++)
			{
				// Sticks
				m_ControllerRightVertical = axes[5];
				m_ControllerRightHorizontal = axes[2];

				m_ControllerLeftVertical = axes[1];
				m_ControllerLeftHorizontal = axes[0];

				// Bumpers
				m_ControllerLeftBumper = axes[3];
				m_ControllerRightBumper = axes[4];
			}
			// Buttons
			m_ControllerButtonsMapping = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &m_ControllerButtonsCount);

			// Debugging
			//for (int i = 0; i < m_ControllerButtonsCount; i++)
			//{
			//	if (buttons[i] == GLFW_PRESS)
			//	{
			//		CHROMA_INFO("Index : {0}", i);
			//	}
			//}
		}
		else
		{
			//CHROMA_WARN("INPUT :: No Controller Connected");
		}
	}

	glm::vec3 Input::ScreenToWorldRay(float const& mouseX, float const& mouseY)
	{
		// get Window dimensions
		glfwGetWindowSize(m_Window, &m_ScreenWidth, &m_ScreenHeight);

		//calculate NDC Coordinates
		float x = (2.0f * mouseX) / m_ScreenWidth - 1.0f;
		float y = 1.0f - (2.0f * mouseY) / m_ScreenHeight;
		float z = 1.0f;

		// Normalized Device Coordinates
		glm::vec3 ray_nds(x, y, z);

		// Homogenous Clip Coordinates
		glm::vec4 ray_clip(ray_nds.x, ray_nds.y, -1.0f, 1.0f);

		// Eye Coordinates
		// we bring the homogenous clip coordinates into eye space using the 
		// inverse of the projection matrix
		glm::vec4 ray_eye = glm::inverse(m_Camera->GetProjectionMatrix()) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

		// World Coordinates
		// we bring the view/eye coordinates into world space with the 
		// inverse of the view matrix
		glm::vec3 ray_world = glm::vec3((glm::inverse(m_Camera->GetViewMatrix()) * ray_eye));
		ray_world = glm::normalize(ray_world);

		return ray_world;
	}


	Input::Input(GLFWwindow*& windowVal)
	{
		m_Window = windowVal;
		Initialize();
	}

	Input::~Input()
	{
	}

}
