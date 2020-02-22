#ifndef _CHROMA_INPUT_
#define _CHROMA_INPUT_


//common
#include <common/PrecompiledHeader.h>
// chroma
#include <camera/Camera.h>
#include <common/CoreCommon.h>
#include <input/MouseCodes.h>


class Camera;

namespace Chroma
{
	class Input
	{
		// components
		static Camera* m_Camera;

		// attrs
		// MOUSE
		static bool m_CursorEnabled;

		static float m_CaptureMouseX, m_CaptureMouseY;
		static float m_MouseX, m_MouseY;
		static glm::vec2 m_MouseXYOffset;

		static float m_LastMouseX, m_LastMouseY;
		static float m_PickedMouseX;
		static float m_PickedMouseY;
		static glm::vec3 m_LastMouseRay;

		static int m_ScreenWidth, m_ScreenHeight;
		static void m_MousePickerCallback();

		// CONTROLLER
		static bool m_ControllerEnabled;

		static int m_ControllerAxesCount;
		static int m_ControllerButtonsCount;
		static const unsigned char* m_ControllerButtonsMapping;

		static float m_ControllerRightVertical;
		static float m_ControllerRightHorizontal;

		static float m_ControllerLeftVertical;
		static float m_ControllerLeftHorizontal;

		static float m_ControllerLeftBumper;
		static float m_ControllerRightBumper;

		// functions 
		static void UpdateMouseCoordinates();
		static void UpdateMousePicker();
		static void UpdateController();

		static glm::vec3 ScreenToWorldRay(float const& mouseX, float const& mouseY);

		// callbacks
		static void mouse_aim_callback(GLFWwindow* window, double xpos, double ypos);
		static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);


	public:
		enum Key {
			LEFT_SHIFT, RIGHT_SHIFT, LEFT_ALT, RIGHT_ALT, SPACEBAR, ESCAPE, A, C, D, E, Q, S, W, Y, P,
			NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,
			LEFT_MOUSE, LEFT_CTRL, RIGHT_CTRL, RIGHT_MOUSE, MIDDLE_MOUSE, LEFT_MOUSE_RELEASE, RIGHT_MOUSE_RELEASE,
			MIDDLE_MOUSE_RELEASE,
			CROSS, SQUARE, CIRCLE, TRIANGLE, R1, L1, L3, R3, R2, L2, DPADLEFT, DPADRIGHT, DPADUP, DPADDOWN, OPTIONS,
			SHARE, TOUCHPAD
		};
		bool static IsPressed(Key KeySelection);

		//  functions
		static void Init();
		static void Update();

		// getters and setters
		// MOUSE
		// mouse xy
		inline static glm::vec2 GetMouseXY() { return glm::vec2(m_MouseX, m_MouseY); }
		inline static glm::vec2 GetMouseXYOffset() { return m_MouseXYOffset; }
		// cursor attrs
		inline static bool GetCursorEnabled() { return m_CursorEnabled; }
		inline static glm::vec3 GetLastRay() { return m_LastMouseRay; }

		// CONTROLLER
		inline static bool GetControllerEnabled() { return m_ControllerEnabled; }
		// Sticks
		inline static float& GetControllerLeftVertical() { return m_ControllerLeftVertical; }
		inline static float& GetControllerLeftHorizontal() { return m_ControllerLeftHorizontal; }

		inline static float& GetControllerRightVertical() { return m_ControllerRightVertical; }
		inline static float& GetControllerRightHorizontal() { return m_ControllerRightHorizontal; }
		// Bumpers
		inline static float& GetControllerRightBumper() { return m_ControllerRightBumper; }
		inline static float& GetControllerLeftBumper() { return m_ControllerLeftBumper; }

		// bind
		static void BindCamera(Camera* cam) { m_Camera = cam; };

		// constructors
		Input() {};
		~Input();
	};
}


#define CROSS_MAPPING 1
#define SQUARE_MAPPING 0
#define CIRCLE_MAPPING 2
#define TRIANGLE_MAPPING 3
#define L1_MAPPING 4
#define R1_MAPPING 5
#define L2_MAPPING 6
#define R2_MAPPING 7
#define L3_MAPPING 10
#define R3_MAPPING 11
#define DPADRIGHT_MAPPING 15
#define DPADLEFT_MAPPING 17
#define DPADUP_MAPPING 14
#define DPADDOWN_MAPPING 16
#define SHARE_MAPPING 8
#define OPTIONS_MAPPING 9
#define TOUCHPAD_MAPPING 13


#endif