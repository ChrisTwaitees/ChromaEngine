#ifndef CHROMA_WINDOWS_WINDOW_H
#define CHROMA_WINDOWS_WINDOW_H

#include <window/Window.h>
#include <context/GraphicsContext.h>

#include <GLFW/glfw3.h>

namespace Chroma
{

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void OnUpdate();

		virtual uint32_t GetWidth() const override { return m_Data.Width; }
		virtual uint32_t GetHeight() const override { return m_Data.Height; }

		virtual bool GetCursorEnabled() override { return m_CursorEnabled; }
		virtual void ToggleCursorEnabled() override;
		virtual void SetCursorEnabled(const bool& enabled) override;
		virtual glm::vec2 GetCursorCoordinates() override;

		// Window Attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled);
		virtual bool IsVSync() const override;

		virtual void* GetNativeWindow() override { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		std::unique_ptr<GraphicsContext> m_Context;

		bool m_CursorEnabled{ true };


		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			float MouseXPos, MouseYPos;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}



# endif