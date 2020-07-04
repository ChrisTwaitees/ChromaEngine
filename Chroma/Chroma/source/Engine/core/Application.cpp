#include "Application.h"

#include <engine/Engine.h>
#include <editor/Editor.h>


namespace Chroma
{

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		CHROMA_PROFILE_FUNCTION();

		// ensure application is singleton
		CHROMA_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// logging
		Log::Init();

		// TEMP
		Render::SetAPI(Render::API::OpenGL);

		// create window
		m_Window = Window::Create(WindowProps(name));
		// Subscribe Application OnEvent to window's eventcallback
		m_Window->SetEventCallback(CHROMA_BIND_EVENT_FN(Application::OnEvent));

		// engine
		Engine::Init();

		// editor
		Editor::Init();

		// scene
		Scene::PreSceneBuild();

		// populate scene
		Editor::Sponza();

		// post scene build
		Scene::PostSceneBuild();
	}

	Application::~Application()
	{
		CHROMA_PROFILE_FUNCTION();

	}

	void Application::OnEvent(Event& e)
	{
		CHROMA_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(CHROMA_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(CHROMA_BIND_EVENT_FN(Application::OnWindowResize));
		dispatcher.Dispatch<KeyPressedEvent>(CHROMA_BIND_EVENT_FN(Application::OnKeyPressed));

	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		CHROMA_PROFILE_FUNCTION();

		while (m_Running)
		{
			CHROMA_PROFILE_SCOPE("Main Run Loop");
			
			// ENGINE TICK
			Chroma::Engine::OnUpdate();

			// EDITOR TICK
			Chroma::Editor::OnUpdate();

			m_Window->OnUpdate();
		}

	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;

		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		CHROMA_INFO("Dispatching a WindowResize Command");
		return false;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		// check if application should close
		if (e.GetKeyCode() == KeyCode::Escape)
			Close();

		// TEMP : trigger physics debug
		if(e.GetKeyCode() == KeyCode::P)
			Chroma::Physics::ToggleDrawDebug();

		// TEMP : toggle mouse enabled
		if (e.GetKeyCode() == KeyCode::LeftControl)
			m_Window->ToggleCursorEnabled();

		return true;
	}



	Application* CreateApplication()
	{
		return new Application();
	}

}



