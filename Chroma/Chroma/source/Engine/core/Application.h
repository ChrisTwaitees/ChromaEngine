
#ifndef CHROMA_APPLICATION_H
#define CHROMA_APPLICATION_H

#include <common/PrecompiledHeader.h>

#include <core/Base.h>
#include <event/Event.h>
#include <event/ApplicationEvent.h>
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>
#include <window/Window.h>

int main();

namespace Chroma
{
	class Application
	{
	public:
		Application(const std::string& name = "Chroma App");
		virtual ~Application();

		void OnEvent(Event& e);

		// consider layering as suggested in Cherno video here : https://www.youtube.com/watch?v=_Kj6BSfM6P4&t=909s
		Window& GetWindow() { return *m_Window; }

		void Close();

		static Application& Get() { return *s_Instance; }

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

		std::unique_ptr<Window> m_Window;

		bool m_Running = true;
		bool m_Minimized = false;

		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
		friend int ::main();

	};

	// To be defined in CLIENT
	Application* CreateApplication();

}


#endif // CHROMA_APPLICATION_H