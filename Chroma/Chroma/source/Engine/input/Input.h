#ifndef _CHROMA_INPUT_
#define _CHROMA_INPUT_

//#ifndef PS4_CONTROLLER
//#define PS4_CONTROLLER
//#endif

#ifndef XBOX_CONTROLLER
#define XBOX_CONTROLLER
#endif

//common
#include <common/PrecompiledHeader.h>
// chroma
#include <camera/Camera.h>
#include <common/CoreCommon.h>
#include <input/MouseCodes.h>
#include <input/KeyCodes.h>
#include <input/ControllerCodes.h>

#include <event/Event.h>
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>


namespace Chroma
{
	class Input
	{
	public:
		void OnEvent(Event& e);

		// MOUSE
		// mouse xy
		static glm::vec2 GetMouseCoordinates();
		static glm::vec2 GetMouseXYOffset() {return glm::vec2(m_CurrentMouseX - m_LastMouseX, m_CurrentMouseY - m_LastMouseY); }
		// cursor attrs
		static void ToggleCursorEnabled();
		static void SetCursorEnabled(bool const& enabledState) ;
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

		// Axis
		static float GetAxis(const char* axis);

		// constructors
		Input() {};
		~Input() = default;

	private:


		static bool OnMousePressed(MouseButtonPressedEvent& e);
		static bool OnMouseMoved(MouseMovedEvent& e);

		static float m_CurrentMouseX, m_CurrentMouseY;
		static float m_LastMouseX, m_LastMouseY;

		static glm::vec3 m_LastMouseRay;

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
		static void UpdateController();

		static glm::vec3 ScreenToWorldRay(const glm::vec2& mouseCoordinates);
	};
}

#ifdef PS4_CONTROLLER
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
// Axes
#define LH_AXIS 0
#define LV_AXIS 1
#define RH_AXIS 2
#define RV_AXIS 5
#define L2_AXIS 3
#define R2_AXIS 4
#endif

#ifdef XBOX_CONTROLLER
// Buttons
#define CROSS_MAPPING 0
#define SQUARE_MAPPING 2
#define CIRCLE_MAPPING 1
#define TRIANGLE_MAPPING 3
#define L1_MAPPING 4
#define R1_MAPPING 5
#define L2_MAPPING 6
#define R2_MAPPING 7
#define L3_MAPPING 8
#define R3_MAPPING 9
#define DPADRIGHT_MAPPING 11
#define DPADLEFT_MAPPING 13
#define DPADUP_MAPPING 10
#define DPADDOWN_MAPPING 12
#define SHARE_MAPPING 6
#define OPTIONS_MAPPING 7
#define TOUCHPAD_MAPPING 99
// Axes
#define LH_AXIS 0
#define LV_AXIS 1
#define RH_AXIS 2
#define RV_AXIS 3
#define L2_AXIS 4
#define R2_AXIS 5
#endif

#endif