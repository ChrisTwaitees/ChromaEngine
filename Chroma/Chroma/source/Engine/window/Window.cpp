#include "Window.h"

#ifdef CHROMA_PLATFORM_WINDOWS
#include <Platform/Windows/WindowsWindow.h>
#endif


namespace Chroma
{
	std::unique_ptr<Window> Window::Create(const WindowProps& props)
	{
	#ifdef CHROMA_PLATFORM_WINDOWS

		return std::make_unique<WindowsWindow>(props);
	#else
		CHROMA_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}
}
