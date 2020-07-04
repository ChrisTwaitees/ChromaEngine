#include "Engine.h"


namespace Chroma
{

#ifdef DEBUG
	bool Engine::m_DrawProfilingWindow;
#endif

	void Engine::Update()
	{
		CHROMA_PROFILE_FUNCTION();
		// Scene
		Chroma::Scene::Update();


		// Physics
		Chroma::Physics::Update();

		// Update Components
		UpdateComponents();
		
		// Resources 
		Chroma::ResourceManager::Update();

	}

	void Engine::UpdateComponents()
	{
		CHROMA_PROFILE_FUNCTION();
		// Animation Components
		//Chroma::JobSystem::Execute([] {
		//	for (UID const& ComponentUID : Chroma::Scene::GetAnimationComponentUIDs()) 
		//	{
		//		Chroma::Scene::GetComponent(ComponentUID)->Update();
		//	}});

		// Character Controller Components
		//Chroma::JobSystem::Execute([] {
		//	for (UID const& ComponentUID : Chroma::Scene::GetCharacterControllerUIDs())
		//	{
		//		Chroma::Scene::GetComponent(ComponentUID)->Update();
		//	}});


		//// IK Components
		//Chroma::JobSystem::Execute([] {
		//	for (UID const& ComponentUID : Chroma::Scene::GetIKComponentUIDs())
		//	{
		//		Chroma::Scene::GetComponent(ComponentUID)->Update();
		//	}});


		//// UI Components
		//Chroma::JobSystem::Execute([] {
		//	for (UID const& ComponentUID : Chroma::Scene::GetUIComponentUIDs())
		//	{
		//		Chroma::Scene::GetComponent(ComponentUID)->Update();
		//	}});

		// Animation Components
		for (UID const& ComponentUID : Chroma::Scene::GetAnimationComponentUIDs())
		{
			Chroma::Scene::GetComponent(ComponentUID)->Update();
		}

		// Character Controller Components
		for (UID const& ComponentUID : Chroma::Scene::GetCharacterControllerUIDs())
		{
			Chroma::Scene::GetComponent(ComponentUID)->Update();
		}

		// IK Components
		for (UID const& ComponentUID : Chroma::Scene::GetIKComponentUIDs())
		{
			Chroma::Scene::GetComponent(ComponentUID)->Update();
		}

		// UI Components
		for (UID const& ComponentUID : Chroma::Scene::GetUIComponentUIDs())
		{
			Chroma::Scene::GetComponent(ComponentUID)->Update();
		}
	}

	void Engine::Draw()
	{
		CHROMA_PROFILE_FUNCTION();
		Chroma::Render::RenderScene();

		Chroma::UI::Draw();
		//Chroma::Screen::Update();
	}

	void Engine::OnUpdate()
	{
#ifdef DEBUG
#ifndef EDITOR

		CHROMA_PROFILE_BEGIN_SESSION("Engine");
#endif
#endif
		CHROMA_PROFILE_FUNCTION();
		// update time
		Chroma::Time::Update();

		// process input
		ProcessInput();

		// Update components
		Update();

		// update while lag is less than framerate cap
		//while (Chroma::Time::GetLag() >= Chroma::Time::GetMSPerFrame())
		//{
		//	Update();
		//	Chroma::Time::DecreaseLag(Chroma::Time::GetMSPerFrame());
		//}
		// consider Sleep if Render misaligning with update https://dewitters.com/dewitters-gameloop/

		// Render Scene
		Chroma::JobSystem::Wait();
		Draw();

#ifdef DEBUG
#ifndef EDITOR

		if (Chroma::Input::IsPressed(Chroma::Input::TAB))
			m_DrawProfilingWindow = m_DrawProfilingWindow ? false : true;

		if (m_DrawProfilingWindow)
			Chroma::UI::ShowProfilingWindow();
		CHROMA_PROFILE_END_SESSION();
#endif
#endif

	}

	void Engine::Init()
	{
		// job system
		Chroma::JobSystem::Initialize();

		// resource manager
		Chroma::ResourceManager::Init();

		// scene
		Chroma::Scene::Init();

		// Time
		Chroma::JobSystem::Execute(Chroma::Time::Init);
		CHROMA_INFO("Time Initialized.");

		// Renderer
		Chroma::Render::Init();
		CHROMA_INFO("Renderer Initialized.");

		// GUI
		Chroma::JobSystem::Execute(Chroma::UI::Init);
		CHROMA_INFO("GUI Initialized.");

		// PhysicsEngine
		Chroma::JobSystem::Execute(Chroma::Physics::Init);
		Chroma::JobSystem::Wait();
		Chroma::Physics::BindDebugBuffer(Chroma::Render::GetDebugBuffer());
		CHROMA_INFO("Physics Initialized.");

		// Final
		CHROMA_INFO_UNDERLINE;
		CHROMA_INFO("Chroma Engine Successfully Initialized.");
		CHROMA_INFO_UNDERLINE;

	}

	void Engine::ProcessInput()
	{
		CHROMA_PROFILE_FUNCTION();

		// Camera
		Chroma::Scene::GetRenderCamera()->Update();
	}
}


