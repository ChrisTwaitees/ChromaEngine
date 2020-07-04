#ifndef CHROMA_MOUSE_CODES_H
#define CHROMA_MOUSE_CODES_H



namespace Chroma
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define CHROMA_MOUSE_BUTTON_0      ::Chroma::Mouse::Button0
#define CHROMA_MOUSE_BUTTON_1      ::Chroma::Mouse::Button1
#define CHROMA_MOUSE_BUTTON_2      ::Chroma::Mouse::Button2
#define CHROMA_MOUSE_BUTTON_3      ::Chroma::Mouse::Button3
#define CHROMA_MOUSE_BUTTON_4      ::Chroma::Mouse::Button4
#define CHROMA_MOUSE_BUTTON_5      ::Chroma::Mouse::Button5
#define CHROMA_MOUSE_BUTTON_6      ::Chroma::Mouse::Button6
#define CHROMA_MOUSE_BUTTON_7      ::Chroma::Mouse::Button7
#define CHROMA_MOUSE_BUTTON_LAST   ::Chroma::Mouse::ButtonLast
#define CHROMA_MOUSE_BUTTON_LEFT   ::Chroma::Mouse::ButtonLeft
#define CHROMA_MOUSE_BUTTON_RIGHT  ::Chroma::Mouse::ButtonRight
#define CHROMA_MOUSE_BUTTON_MIDDLE ::Chroma::Mouse::ButtonMiddle

#endif