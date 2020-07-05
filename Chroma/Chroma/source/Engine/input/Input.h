#ifndef _CHROMA_INPUT_
#define _CHROMA_INPUT_


//common
#include <common/PrecompiledHeader.h>
// chroma
#include <camera/Camera.h>
#include <common/CoreCommon.h>
#include <input/MouseCodes.h>
#include <input/KeyCodes.h>
#include <input/ControllerCodes.h>






namespace Chroma
{
	class KeyReleasedEvent;
	class MouseButtonPressedEvent;
	class MouseMovedEvent;
		

	class Input
	{
	public:
		static void OnEvent(Event& e);

		// keyboard
		static bool IsPressed(const KeyCode& keyCode);
		static bool IsReleased(const KeyCode& keyCode);
		static bool IsHeld(const KeyCode& keyCode);

		// mouse
		static bool IsPressed(const MouseCode& keyCode);
		static bool IsReleased(const MouseCode& keyCode);
		static bool IsHeld(const MouseCode& keyCode);

		// MOUSE
		// mouse xy
		static glm::vec2 GetMouseCoordinates();
		static glm::vec2 GetMouseXYOffset() {return glm::vec2(m_CurrentMouseX - m_LastMouseX,  m_LastMouseY - m_CurrentMouseY ); }
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
		// callbacks
		static bool OnMousePressed(MouseButtonPressedEvent& e);
		static bool OnMouseMoved(MouseMovedEvent& e);

		static bool OnKeyReleased(KeyReleasedEvent& e);

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



#endif