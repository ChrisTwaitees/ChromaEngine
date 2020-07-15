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

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;

		virtual uint32_t GetXPos() const override { return m_Data.XPos; }
		virtual uint32_t GetYPos() const override { return m_Data.YPos; }

		//input 
		virtual bool IsPressed(const KeyCode& keyCode) override;
		virtual bool IsReleased(const KeyCode& keyCode) override;
		virtual bool IsHeld(const KeyCode& keyCode) override;

		// mouse
		virtual bool IsPressed(const MouseCode& keyCode) override;
		virtual bool IsReleased(const MouseCode& keyCode) override;
		virtual bool IsHeld(const MouseCode& keyCode)  override;

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
			int XPos, YPos;
			float MouseXPos, MouseYPos;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}



# endif