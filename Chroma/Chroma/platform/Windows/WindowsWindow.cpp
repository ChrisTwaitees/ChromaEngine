#include "WindowsWindow.h"
#include <render/Render.h>

// events
#include <event/ApplicationEvent.h>
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>

// keycodes
#include <input/KeyCodes.h>
#include <input/MouseCodes.h>

namespace Chroma
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		CHROMA_ERROR("GLFW ERROR : ({0}) : {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		CHROMA_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		CHROMA_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		CHROMA_PROFILE_FUNCTION();

		// set data
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		// ensure no other glfw windows are already open
		if (s_GLFWWindowCount == 0)
		{
			CHROMA_INFO("GLFW Init");
			int success = glfwInit();
			CHROMA_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		// enable opengl debug functionality if available
		#ifdef DEBUG
		if (Render::GetAPI() == Render::API::OpenGL)
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		#endif

		// create window
		CHROMA_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		++s_GLFWWindowCount;

		// check window valid
		CHROMA_ASSERT(m_Window, "Failed to create glfw window");
		// capture mouse
		SetCursorEnabled(true);

		// set user pointer
		glfwSetWindowUserPointer(m_Window, &m_Data);

		// create and initialize graphics context
		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		// enable vsync
		SetVSync(true);

		// GLFW CALLBACKS
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(static_cast<KeyCode>(key), 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(static_cast<KeyCode>(key));
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(static_cast<KeyCode>(key), 1);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(static_cast<KeyCode>(keycode));
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(static_cast<MouseCode>(button));
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			data.MouseXPos = (float)xPos;
			data.MouseYPos = (float)yPos;
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});

	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	bool WindowsWindow::IsPressed(const KeyCode& keyCode)
	{
		return glfwGetKey(m_Window, (int)keyCode) == GLFW_PRESS;
	}

	bool WindowsWindow::IsReleased(const KeyCode& keyCode)
	{
		return glfwGetKey(m_Window, (int)keyCode) == GLFW_RELEASE;
	}

	bool WindowsWindow::IsHeld(const KeyCode& keyCode)
	{
		return glfwGetKey(m_Window, (int)keyCode) == GLFW_REPEAT;
	}

	bool WindowsWindow::IsPressed(const MouseCode& mouseCode)
	{
		return glfwGetMouseButton(m_Window, (int)mouseCode) == GLFW_PRESS;
	}

	bool WindowsWindow::IsReleased(const MouseCode& mouseCode)
	{
		return glfwGetMouseButton(m_Window, (int)mouseCode) == GLFW_RELEASE;
	}

	bool WindowsWindow::IsHeld(const MouseCode& mouseCode)
	{
		return glfwGetMouseButton(m_Window, (int)mouseCode) == GLFW_REPEAT;
	}

	void WindowsWindow::ToggleCursorEnabled()
	{
		int cursorMode = glfwGetInputMode(m_Window, GLFW_CURSOR);
		cursorMode == GLFW_CURSOR_DISABLED ? SetCursorEnabled(false) : SetCursorEnabled(true);
	}

	void WindowsWindow::SetCursorEnabled(const bool& enabled)
	{
		m_CursorEnabled = enabled;
		int cursorEnum = m_CursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
		glfwSetInputMode(m_Window, GLFW_CURSOR, cursorEnum);
	}

	glm::vec2 WindowsWindow::GetCursorCoordinates()
	{
		return glm::vec2(m_Data.MouseXPos, m_Data.MouseYPos);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		enabled ? glfwSwapInterval(1) : glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
			glfwTerminate();
	}
}


