#ifndef CHROMA_WINDOW_H
#define CHROMA_WINDOW_H

#include <common/PrecompiledHeader.h>
#include <core/Base.h>
#include <event/Event.h>

namespace Chroma
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Chroma Application",
					uint32_t width = 1280,
					uint32_t height = 720)
			: Title(title), Width(width), Height(height) {}
	};


	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// cursor 
		virtual bool GetCursorEnabled() = 0;
		virtual void ToggleCursorEnabled() = 0;
		virtual void SetCursorEnabled(const bool& enabled) = 0;
		virtual glm::vec2 GetCursorCoordinates() = 0;

		// Window Attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() = 0;

		static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
	};

}

#endif //CHROMA_WINDOW_H

