#include "Input.h"
#include <scene/Scene.h>
#include <physics/PhysicsEngine.h>
#include <UI/core/UI.h>

namespace Chroma
{
	// Window and Camera
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

	float Input::m_MouseX;
	float Input::m_CaptureMouseX;
	float Input::m_CaptureMouseY;
	float Input::m_MouseY;
	float Input::m_LastMouseX;
	float Input::m_LastMouseY;

	float Input::m_PickedMouseX;
	float Input::m_PickedMouseY;
	glm::vec2 Input::m_MouseXYOffset;
	glm::vec3 Input::m_LastMouseRay;

	int Input::m_ScreenWidth;
	int Input::m_ScreenHeight;

	void Input::Init()
	{
		// glfw attach callbacks
		glfwSetCursorPosCallback(Chroma::Screen::GetWindow(), mouse_aim_callback);
		glfwSetScrollCallback(Chroma::Screen::GetWindow(), mouse_scroll_callback);
		glfwSetMouseButtonCallback(Chroma::Screen::GetWindow(), mouse_click_callback);

		// init vars
		m_CursorEnabled = false;
	}

	bool Input::IsPressed(Key KeySelection)
	{
		switch (KeySelection) {
		case LEFT_SHIFT:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
			break;
		case RIGHT_SHIFT:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
			break;
		case LEFT_ALT:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
			break;
		case RIGHT_ALT:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_RIGHT_ALT) == GLFW_PRESS;
			break;
		case SPACEBAR:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS;
			break;
		case ESCAPE:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS;
			break;
		case LEFT_CTRL:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
			break;
		case RIGHT_CTRL:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
			break;
		case A:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_A) == GLFW_PRESS;
			break;
		case C:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_C) == GLFW_PRESS;
			break;
		case D:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_D) == GLFW_PRESS;
			break;
		case E:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_E) == GLFW_PRESS;
			break;
		case H:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_H) == GLFW_PRESS;
			break;
		case J:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_J) == GLFW_PRESS;
			break;
		case P:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_P) == GLFW_PRESS;
			break;
		case Q:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_Q) == GLFW_PRESS;
			break;
		case S:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_S) == GLFW_PRESS;
			break;
		case Y:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_Y) == GLFW_PRESS;
			break;
		case W:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_W) == GLFW_PRESS;
			break;
		case NUM0:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_0) == GLFW_PRESS;
			break;
		case NUM1:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_1) == GLFW_PRESS;
			break;
		case NUM2:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_2) == GLFW_PRESS;
			break;
		case NUM3:
			return glfwGetKey(Chroma::Screen::GetWindow(), GLFW_KEY_3) == GLFW_PRESS;
			break;
		case LEFT_MOUSE:
			return glfwGetMouseButton(Chroma::Screen::GetWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
			break;
		case RIGHT_MOUSE:
			return glfwGetMouseButton(Chroma::Screen::GetWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS;
			break;
		case MIDDLE_MOUSE:
			return glfwGetMouseButton(Chroma::Screen::GetWindow(), GLFW_MOUSE_BUTTON_3) == GLFW_PRESS;
			break;
		case LEFT_MOUSE_RELEASE:
			return glfwGetMouseButton(Chroma::Screen::GetWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE;
			break;
		case RIGHT_MOUSE_RELEASE:
			return glfwGetMouseButton(Chroma::Screen::GetWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE;
			break;
		case MIDDLE_MOUSE_RELEASE:
			return glfwGetMouseButton(Chroma::Screen::GetWindow(), GLFW_MOUSE_BUTTON_3) == GLFW_RELEASE;
			break;
		}
		if (m_ControllerButtonsMapping != nullptr)
		{
			switch (KeySelection) {
			case CROSS:
				return m_ControllerButtonsMapping[CROSS_MAPPING] == GLFW_PRESS;
				break;
			case SQUARE:
				return m_ControllerButtonsMapping[SQUARE_MAPPING] == GLFW_PRESS;
				break;
			case CIRCLE:
				return m_ControllerButtonsMapping[CIRCLE_MAPPING] == GLFW_PRESS;
				break;
			case TRIANGLE:
				return m_ControllerButtonsMapping[TRIANGLE_MAPPING] == GLFW_PRESS;
				break;
			case R1:
				return m_ControllerButtonsMapping[R1_MAPPING] == GLFW_PRESS;
				break;
			case L1:
				return m_ControllerButtonsMapping[L1_MAPPING] == GLFW_PRESS;
				break;
			case R2:
				return m_ControllerButtonsMapping[R2_MAPPING] == GLFW_PRESS;
				break;
			case R3:
				return m_ControllerButtonsMapping[R3_MAPPING] == GLFW_PRESS;
				break;
			case L2:
				return m_ControllerButtonsMapping[L2_MAPPING] == GLFW_PRESS;
				break;
			case L3:
				return m_ControllerButtonsMapping[L3_MAPPING] == GLFW_PRESS;
				break;
			case DPADLEFT:
				return m_ControllerButtonsMapping[DPADLEFT_MAPPING] == GLFW_PRESS;
				break;
			case DPADRIGHT:
				return m_ControllerButtonsMapping[DPADRIGHT_MAPPING] == GLFW_PRESS;
				break;
			case DPADUP:
				return m_ControllerButtonsMapping[DPADUP_MAPPING] == GLFW_PRESS;
				break;
			case DPADDOWN:
				return m_ControllerButtonsMapping[DPADDOWN_MAPPING] == GLFW_PRESS;
				break;
			case OPTIONS:
				return m_ControllerButtonsMapping[OPTIONS_MAPPING] == GLFW_PRESS;
				break;
			case SHARE:
				return m_ControllerButtonsMapping[SHARE_MAPPING] == GLFW_PRESS;
				break;
			case TOUCHPAD:
				return m_ControllerButtonsMapping[TOUCHPAD_MAPPING] == GLFW_PRESS;
				break;
			}
		}

		return false;
	}

	void Input::Update()
	{
		// Should Close?
		UpdateScreen();

		// Mouse
		UpdateMouse();

		// Controller
		UpdateController();

		// Camera
		UpdateCamera();

	}

	void Input::ToggleCursorEnabledState()
	{
		int cursorMode = glfwGetInputMode(Chroma::Screen::GetWindow(), GLFW_CURSOR);
		cursorMode == GLFW_CURSOR_DISABLED ? SetCursorEnabled(false) : SetCursorEnabled(true);
	}

	void Input::SetCursorEnabled(bool const& enabledState)
	{
		m_CursorEnabled = enabledState;
		m_CursorEnabled ? glfwSetInputMode(Chroma::Screen::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED) : glfwSetInputMode(Chroma::Screen::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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

	void Input::m_MousePickerCallback()
	{
		// Ray Interest Test
		glm::vec3 start = Chroma::Scene::GetRenderCamera()->GetPosition();
		glm::vec3 end = start + (Chroma::Input::GetLastRay() * glm::vec3(1000.0));
		IEntity* clickedEntity = Chroma::Physics::GetEntityRayTest(start, end);
		if (clickedEntity)
			Chroma::UI::SetSelectedEntityName(clickedEntity->GetName());
	}

	void Input::UpdateMouseCoordinates()
	{
		// current coordinates
		m_MouseX = m_CaptureMouseX;
		m_MouseY = m_CaptureMouseY;

		// m_Offset
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

	void Input::UpdateScreen()
	{
		if (IsPressed(ESCAPE)) {
			glfwSetWindowShouldClose(Chroma::Screen::GetWindow(), true);
			return;
		}
	}

	void Input::UpdateController()
	{
		m_ControllerEnabled = (glfwJoystickPresent(GLFW_JOYSTICK_1) > 0) ? true : false;

		if (m_ControllerEnabled)
		{
			// Axes
			const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &m_ControllerAxesCount);
			for (int i = 0; i < m_ControllerAxesCount; i++)
			{
				// Sticks
				m_ControllerRightVertical = axes[RV_AXIS];
				m_ControllerRightHorizontal = axes[RH_AXIS];

				m_ControllerLeftVertical = axes[LV_AXIS];
				m_ControllerLeftHorizontal = axes[LH_AXIS];

				// Bumpers
				m_ControllerLeftBumper = axes[L2_AXIS];
				m_ControllerRightBumper = axes[R2_AXIS];
			}
			// Buttons
			m_ControllerButtonsMapping = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &m_ControllerButtonsCount);

			// DEBUGGING
			//// BUTTONS
			//for (int i = 0; i < m_ControllerButtonsCount; i++)
			//{
			//	if (m_ControllerButtonsMapping[i] == GLFW_PRESS)
			//	{
			//		CHROMA_INFO("Index : {0}", i);
			//	}
			//}
			//AXIS
			//for (int i = 0; i < m_ControllerAxesCount; i++)
			//{
			//	CHROMA_INFO("Axis Index : {0} , Weight : {1}", i, axes[i]);
			//}
		}
		else
		{
			//CHROMA_WARN("INPUT :: No Controller Connected");
		}
	}

	void Input::UpdateMouse()
	{
		// Capture / release mouse
		if (IsPressed(LEFT_CTRL))
		{
			ToggleCursorEnabledState();
		}

		// Mouse Coordinates
		if (m_CursorEnabled)
		{
		}
		UpdateMouseCoordinates();

		// Mouse Picker
		UpdateMousePicker();
	}

	void Input::UpdateCamera()
	{
		if (Chroma::Input::IsPressed(Chroma::Input::NUM1))
			Chroma::Scene::GetRenderCamera()->SetCameraMode(Maya);
		if (Chroma::Input::IsPressed(Chroma::Input::NUM2))
			Chroma::Scene::GetRenderCamera()->SetCameraMode(FlyCam);
		if (Chroma::Input::IsPressed(Chroma::Input::NUM3))
			Chroma::Scene::GetRenderCamera()->SetCameraMode(Custom);
	}

	glm::vec3 Input::ScreenToWorldRay(float const& mouseX, float const& mouseY)
	{
		// get Window dimensions
		glfwGetWindowSize(Chroma::Screen::GetWindow(), &m_ScreenWidth, &m_ScreenHeight);

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


	float Input::GetAxis(const char* axis)
	{
		if (axis == "Horizontal")
		{
			if (m_ControllerEnabled)
			{
				return m_ControllerLeftHorizontal;
			}
			else
			{
				if (IsPressed(W))
					return 1.0f;
				else if (IsPressed(S))
					return -1.0f;
				else
					return 0.0f;
			}
		}
		else if (axis == "Vertical")
		{
			if (m_ControllerEnabled)
			{
				return m_ControllerLeftVertical;
			}
			else
			{
				if (IsPressed(D))
					return 1.0f;
				if (IsPressed(A))
					return -1.0f;
				else
					return 0.0f;
			}
		}
		else
		{
			CHROMA_ERROR("CHROMA INPUT :: Invalid Axis {}", axis);
			throw(std::invalid_argument("Invalid Axis"));
		}
	
	}

	Input::~Input()
	{
	}

}
