#include "Engine.h"


namespace Chroma
{
	void Engine::Update()
	{
		// Physics
		Chroma::Physics::Update();

		// Updating Components
		for (UID const& ComponentUID : Chroma::Scene::GetUpdatingComponentUIDs())
		{
			Chroma::Scene::GetComponent(ComponentUID)->Update();
		}

		// Workers
		IWorker::DoWork();
	}

	void Engine::Draw()
	{
		Chroma::Render::RenderScene();
		Chroma::GUI::Draw();
		Chroma::Screen::Update();
	}

	void Engine::Tick()
	{
		// update time
		Chroma::Time::Update();

		// process input
		ProcessInput();

		// update while lag is less than framerate cap
		while (Chroma::Time::GetLag() >= Chroma::Time::GetMSPerFrame())
		{
			Update();
			Chroma::Time::DecreaseLag(Chroma::Time::GetMSPerFrame());
		}
		// consider Sleep if Render misaligning with update https://dewitters.com/dewitters-gameloop/

		// Render Scene
		Draw();
	}

	void Engine::Init()
	{
		Chroma::Log::Init();

		CHROMA_INFO("------------------------------------------");
		CHROMA_INFO("Chroma Engine Initializing...");
		CHROMA_INFO("------------------------------------------");

		// Core
		Chroma::Core::Init();

		// Scene
		Chroma::Scene::Init();
		CHROMA_INFO("Scene Initialized.");

		// Input
		Chroma::Input::Init();
		Chroma::Input::BindCamera(Chroma::Scene::GetRenderCamera());
		CHROMA_INFO("Input Initialized.");

		// Time
		Chroma::Time::Init();
		CHROMA_INFO("Time Initialized.");

		// Renderer
		Chroma::Render::Init();
		CHROMA_INFO("Renderer Initialized.");

		// GUI
		Chroma::GUI::Init();
		CHROMA_INFO("GUI Initialized.");

		// PhysicsEngine
		Chroma::Physics::Init();
		Chroma::Physics::BindDebugBuffer(Chroma::Render::GetDebugBuffer());
		CHROMA_INFO("Physics Initialized.");

		// Final
		CHROMA_INFO("------------------------------------------");
		CHROMA_INFO("Chroma Engine Successfully Initialized.");
		CHROMA_INFO("------------------------------------------");

	}

	void Engine::ProcessInput()
	{
		// update input
		Chroma::Input::Update();

		// Camera
		Chroma::Scene::GetRenderCamera()->Update();

	}
}


