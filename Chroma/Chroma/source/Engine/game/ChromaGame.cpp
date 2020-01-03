#include "ChromaGame.h"


void ChromaGame::Update()
{
	// Physics
	Chroma::Physics::Update();

	// Animation
	if (Chroma::GUI::debugAnim)
		m_Animation->UpdateDebug(Chroma::GUI::animClipName, Chroma::GUI::DebugAnimClipPos);
	else
		m_Animation->Update();

	// Workers
	IWorker::DoWork();
}

void ChromaGame::Draw()
{
	m_Renderer->RenderScene();
	Chroma::GUI::Draw();
}


void ChromaGame::MousePickerCallback()
{
	// Ray Interest Test
	glm::vec3 start = m_Scene->GetRenderCamera()->GetPosition();
	glm::vec3 end = start + ( Chroma::Input::GetLastRay() * glm::vec3(1000.0));
	IEntity* clickedEntity  = Chroma::Physics::GetEntityRayTest(start, end);
	if (clickedEntity)
		Chroma::GUI::SetSelectedEntityName(clickedEntity->GetName());

}

void ChromaGame::Tick()
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


ChromaGame::~ChromaGame()
{
}

ChromaGame::ChromaGame(Scene*& Scene)
{
	m_Scene = Scene;
	Init();
}

void ChromaGame::Init()
{
	// Logging
	Chroma::Log::Init();
	CHROMA_INFO("Chroma Initializing...");

	// Time
	Chroma::Time::Init();
	CHROMA_INFO("Chroma Time Initialized.");

	// Screen
	Chroma::Screen::Init();
	CHROMA_INFO("Chroma Screen Initialized.");

	// Renderer
	m_Renderer = new Renderer(m_Scene);
	CHROMA_INFO("Chroma Renderer Initialized.");

	// GUI
	Chroma::GUI::Init();
	CHROMA_INFO("Chroma GUI Initialized.");

	// Input
	Chroma::Input::BindCamera(m_Scene->GetRenderCamera());
	Chroma::Input::BindMousePickerCallback(std::bind(&ChromaGame::MousePickerCallback, this));
	CHROMA_INFO("Chroma Input Bound.");

	// PhysicsEngine
	Chroma::Physics::Init();
	Chroma::Physics::BindDebugBuffer(m_Renderer->GetDebugBuffer());
	CHROMA_INFO("Chroma Physics Engine Initialized.");

	// AnimationEngine
	m_Animation->BindScene(m_Scene);
	CHROMA_INFO("Chroma Animation Engine Initialized.");

}

void ChromaGame::ProcessInput()
{
	// update input
	Chroma::Input::Update();

	// update camera
	if (Chroma::GUI::cameraSelected == 0)
		m_Scene->GetRenderCamera()->SetCameraMode(Maya);
	if (Chroma::GUI::cameraSelected == 1)
		m_Scene->GetRenderCamera()->SetCameraMode(FlyCam);
	m_Scene->GetRenderCamera()->ProcessInput();

	// render physics debug 
	if (Chroma::GUI::drawPhysicsDebug)
		Chroma::Physics::DrawDebug();

	// animation debug
	//if (Chroma::GUI::DebugAnim)
	m_Renderer->GetDebugBuffer()->DrawSceneSkeletons(m_Scene);

}
