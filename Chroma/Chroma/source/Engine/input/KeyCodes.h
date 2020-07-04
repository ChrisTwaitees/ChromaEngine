#ifndef CHROMA_KEYCODES_H
#define CHROMA_KEYCODES_H

namespace Chroma
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define CHROMA_KEY_SPACE           ::Chroma::Key::Space
#define CHROMA_KEY_APOSTROPHE      ::Chroma::Key::Apostrophe    /* ' */
#define CHROMA_KEY_COMMA           ::Chroma::Key::Comma         /* , */
#define CHROMA_KEY_MINUS           ::Chroma::Key::Minus         /* - */
#define CHROMA_KEY_PERIOD          ::Chroma::Key::Period        /* . */
#define CHROMA_KEY_SLASH           ::Chroma::Key::Slash         /* / */
#define CHROMA_KEY_0               ::Chroma::Key::D0
#define CHROMA_KEY_1               ::Chroma::Key::D1
#define CHROMA_KEY_2               ::Chroma::Key::D2
#define CHROMA_KEY_3               ::Chroma::Key::D3
#define CHROMA_KEY_4               ::Chroma::Key::D4
#define CHROMA_KEY_5               ::Chroma::Key::D5
#define CHROMA_KEY_6               ::Chroma::Key::D6
#define CHROMA_KEY_7               ::Chroma::Key::D7
#define CHROMA_KEY_8               ::Chroma::Key::D8
#define CHROMA_KEY_9               ::Chroma::Key::D9
#define CHROMA_KEY_SEMICOLON       ::Chroma::Key::Semicolon     /* ; */
#define CHROMA_KEY_EQUAL           ::Chroma::Key::Equal         /* = */
#define CHROMA_KEY_A               ::Chroma::Key::A
#define CHROMA_KEY_B               ::Chroma::Key::B
#define CHROMA_KEY_C               ::Chroma::Key::C
#define CHROMA_KEY_D               ::Chroma::Key::D
#define CHROMA_KEY_E               ::Chroma::Key::E
#define CHROMA_KEY_F               ::Chroma::Key::F
#define CHROMA_KEY_G               ::Chroma::Key::G
#define CHROMA_KEY_H               ::Chroma::Key::H
#define CHROMA_KEY_I               ::Chroma::Key::I
#define CHROMA_KEY_J               ::Chroma::Key::J
#define CHROMA_KEY_K               ::Chroma::Key::K
#define CHROMA_KEY_L               ::Chroma::Key::L
#define CHROMA_KEY_M               ::Chroma::Key::M
#define CHROMA_KEY_N               ::Chroma::Key::N
#define CHROMA_KEY_O               ::Chroma::Key::O
#define CHROMA_KEY_P               ::Chroma::Key::P
#define CHROMA_KEY_Q               ::Chroma::Key::Q
#define CHROMA_KEY_R               ::Chroma::Key::R
#define CHROMA_KEY_S               ::Chroma::Key::S
#define CHROMA_KEY_T               ::Chroma::Key::T
#define CHROMA_KEY_U               ::Chroma::Key::U
#define CHROMA_KEY_V               ::Chroma::Key::V
#define CHROMA_KEY_W               ::Chroma::Key::W
#define CHROMA_KEY_X               ::Chroma::Key::X
#define CHROMA_KEY_Y               ::Chroma::Key::Y
#define CHROMA_KEY_Z               ::Chroma::Key::Z
#define CHROMA_KEY_LEFT_BRACKET    ::Chroma::Key::LeftBracket   /* [ */
#define CHROMA_KEY_BACKSLASH       ::Chroma::Key::Backslash     /* \ */
#define CHROMA_KEY_RIGHT_BRACKET   ::Chroma::Key::RightBracket  /* ] */
#define CHROMA_KEY_GRAVE_ACCENT    ::Chroma::Key::GraveAccent   /* ` */
#define CHROMA_KEY_WORLD_1         ::Chroma::Key::World1        /* non-US #1 */
#define CHROMA_KEY_WORLD_2         ::Chroma::Key::World2        /* non-US #2 */

/* Function keys */
#define CHROMA_KEY_ESCAPE          ::Chroma::Key::Escape
#define CHROMA_KEY_ENTER           ::Chroma::Key::Enter
#define CHROMA_KEY_TAB             ::Chroma::Key::Tab
#define CHROMA_KEY_BACKSPACE       ::Chroma::Key::Backspace
#define CHROMA_KEY_INSERT          ::Chroma::Key::Insert
#define CHROMA_KEY_DELETE          ::Chroma::Key::Delete
#define CHROMA_KEY_RIGHT           ::Chroma::Key::Right
#define CHROMA_KEY_LEFT            ::Chroma::Key::Left
#define CHROMA_KEY_DOWN            ::Chroma::Key::Down
#define CHROMA_KEY_UP              ::Chroma::Key::Up
#define CHROMA_KEY_PAGE_UP         ::Chroma::Key::PageUp
#define CHROMA_KEY_PAGE_DOWN       ::Chroma::Key::PageDown
#define CHROMA_KEY_HOME            ::Chroma::Key::Home
#define CHROMA_KEY_END             ::Chroma::Key::End
#define CHROMA_KEY_CAPS_LOCK       ::Chroma::Key::CapsLock
#define CHROMA_KEY_SCROLL_LOCK     ::Chroma::Key::ScrollLock
#define CHROMA_KEY_NUM_LOCK        ::Chroma::Key::NumLock
#define CHROMA_KEY_PRINT_SCREEN    ::Chroma::Key::PrintScreen
#define CHROMA_KEY_PAUSE           ::Chroma::Key::Pause
#define CHROMA_KEY_F1              ::Chroma::Key::F1
#define CHROMA_KEY_F2              ::Chroma::Key::F2
#define CHROMA_KEY_F3              ::Chroma::Key::F3
#define CHROMA_KEY_F4              ::Chroma::Key::F4
#define CHROMA_KEY_F5              ::Chroma::Key::F5
#define CHROMA_KEY_F6              ::Chroma::Key::F6
#define CHROMA_KEY_F7              ::Chroma::Key::F7
#define CHROMA_KEY_F8              ::Chroma::Key::F8
#define CHROMA_KEY_F9              ::Chroma::Key::F9
#define CHROMA_KEY_F10             ::Chroma::Key::F10
#define CHROMA_KEY_F11             ::Chroma::Key::F11
#define CHROMA_KEY_F12             ::Chroma::Key::F12
#define CHROMA_KEY_F13             ::Chroma::Key::F13
#define CHROMA_KEY_F14             ::Chroma::Key::F14
#define CHROMA_KEY_F15             ::Chroma::Key::F15
#define CHROMA_KEY_F16             ::Chroma::Key::F16
#define CHROMA_KEY_F17             ::Chroma::Key::F17
#define CHROMA_KEY_F18             ::Chroma::Key::F18
#define CHROMA_KEY_F19             ::Chroma::Key::F19
#define CHROMA_KEY_F20             ::Chroma::Key::F20
#define CHROMA_KEY_F21             ::Chroma::Key::F21
#define CHROMA_KEY_F22             ::Chroma::Key::F22
#define CHROMA_KEY_F23             ::Chroma::Key::F23
#define CHROMA_KEY_F24             ::Chroma::Key::F24
#define CHROMA_KEY_F25             ::Chroma::Key::F25

/* Keypad */
#define CHROMA_KEY_KP_0            ::Chroma::Key::KP0
#define CHROMA_KEY_KP_1            ::Chroma::Key::KP1
#define CHROMA_KEY_KP_2            ::Chroma::Key::KP2
#define CHROMA_KEY_KP_3            ::Chroma::Key::KP3
#define CHROMA_KEY_KP_4            ::Chroma::Key::KP4
#define CHROMA_KEY_KP_5            ::Chroma::Key::KP5
#define CHROMA_KEY_KP_6            ::Chroma::Key::KP6
#define CHROMA_KEY_KP_7            ::Chroma::Key::KP7
#define CHROMA_KEY_KP_8            ::Chroma::Key::KP8
#define CHROMA_KEY_KP_9            ::Chroma::Key::KP9
#define CHROMA_KEY_KP_DECIMAL      ::Chroma::Key::KPDecimal
#define CHROMA_KEY_KP_DIVIDE       ::Chroma::Key::KPDivide
#define CHROMA_KEY_KP_MULTIPLY     ::Chroma::Key::KPMultiply
#define CHROMA_KEY_KP_SUBTRACT     ::Chroma::Key::KPSubtract
#define CHROMA_KEY_KP_ADD          ::Chroma::Key::KPAdd
#define CHROMA_KEY_KP_ENTER        ::Chroma::Key::KPEnter
#define CHROMA_KEY_KP_EQUAL        ::Chroma::Key::KPEqual

#define CHROMA_KEY_LEFT_SHIFT      ::Chroma::Key::LeftShift
#define CHROMA_KEY_LEFT_CONTROL    ::Chroma::Key::LeftControl
#define CHROMA_KEY_LEFT_ALT        ::Chroma::Key::LeftAlt
#define CHROMA_KEY_LEFT_SUPER      ::Chroma::Key::LeftSuper
#define CHROMA_KEY_RIGHT_SHIFT     ::Chroma::Key::RightShift
#define CHROMA_KEY_RIGHT_CONTROL   ::Chroma::Key::RightControl
#define CHROMA_KEY_RIGHT_ALT       ::Chroma::Key::RightAlt
#define CHROMA_KEY_RIGHT_SUPER     ::Chroma::Key::RightSuper
#define CHROMA_KEY_MENU            ::Chroma::Key::Menu

#endif 