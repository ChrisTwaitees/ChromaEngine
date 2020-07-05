#include "Engine.h"


namespace Chroma
{

	void Engine::UpdateComponents()
	{
		CHROMA_PROFILE_FUNCTION();
		// Animation Components
		//Chroma::JobSystem::Execute([] {
		//	for (UID const& ComponentUID : Chroma::Scene::GetAnimationComponentUIDs()) 
		//	{
		//		Chroma::Scene::GetComponent(ComponentUID)->OnUpdate();
		//	}});

		// Character Controller Components
		//Chroma::JobSystem::Execute([] {
		//	for (UID const& ComponentUID : Chroma::Scene::GetCharacterControllerUIDs())
		//	{
		//		Chroma::Scene::GetComponent(ComponentUID)->OnUpdate();
		//	}});


		//// IK Components
		//Chroma::JobSystem::Execute([] {
		//	for (UID const& ComponentUID : Chroma::Scene::GetIKComponentUIDs())
		//	{
		//		Chroma::Scene::GetComponent(ComponentUID)->OnUpdate();
		//	}});


		//// UI Components
		//Chroma::JobSystem::Execute([] {
		//	for (UID const& ComponentUID : Chroma::Scene::GetUIComponentUIDs())
		//	{
		//		Chroma::Scene::GetComponent(ComponentUID)->OnUpdate();
		//	}});

		// Animation Components
		for (UID const& ComponentUID : Scene::GetAnimationComponentUIDs())
		{
			Scene::GetComponent(ComponentUID)->OnUpdate();
		}

		// Character Controller Components
		for (UID const& ComponentUID : Scene::GetCharacterControllerUIDs())
		{
			Scene::GetComponent(ComponentUID)->OnUpdate();
		}

		// IK Components
		for (UID const& ComponentUID : Scene::GetIKComponentUIDs())
		{
			Scene::GetComponent(ComponentUID)->OnUpdate();
		}

		// UI Components
		for (UID const& ComponentUID : Scene::GetUIComponentUIDs())
		{
			Scene::GetComponent(ComponentUID)->OnUpdate();
		}
	}

	void Engine::Draw()
	{
		CHROMA_PROFILE_FUNCTION();

		// Renderer
		Render::RenderScene();

		// UI
		UI::Draw();
	}

	void Engine::OnUpdate()
	{
		CHROMA_PROFILE_FUNCTION();

		// Scene
		Scene::OnUpdate();

		// Physics
		Chroma::Physics::OnUpdate();

		// OnUpdate Components
		UpdateComponents();

		// Resources 
		ResourceManager::OnUpdate();

		// Draw
		JobSystem::Wait();
		Draw();
	}

	void Engine::Init()
	{
		// job system
		JobSystem::Init();

		// resource manager
		ResourceManager::Init();

		// scene
		Scene::Init();

		// time
		JobSystem::Execute(Time::Init);
		CHROMA_INFO("Time Initialized.");

		// renderer
		Render::Init();

		// ui
		JobSystem::Execute(UI::Init);
		CHROMA_INFO("UI Initialized.");

		// physics
		JobSystem::Execute(Physics::Init);
		JobSystem::Wait();
		Physics::BindDebugBuffer(Render::GetDebugBuffer());
		CHROMA_INFO("Physics Initialized.");

		// log
		CHROMA_INFO_UNDERLINE;
		CHROMA_INFO("Chroma Engine Successfully Initialized.");
		CHROMA_INFO_UNDERLINE;
	}

}


