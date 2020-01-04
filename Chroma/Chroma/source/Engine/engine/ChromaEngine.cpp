#include "ChromaEngine.h"


void ChromaEngine::Update()
{
	// Physics
	Chroma::Physics::Update();

	// Animation
	if (Chroma::GUI::debugAnim)
		m_Animation->UpdateDebug(Chroma::GUI::animClipName, Chroma::GUI::DebugAnimClipPos);
	else
		m_Animation->Update();

	// Updating Components
	for (std::string const& ComponentUID : m_Scene->GetUpdatingComponentUIDs())
	{
		m_Scene->GetComponent(ComponentUID)->Update();
	}

	// Workers
	IWorker::DoWork();
}

void ChromaEngine::Draw()
{
	Chroma::Render::RenderScene();
	Chroma::GUI::Draw();
	Chroma::Screen::Update();
}


void ChromaEngine::MousePickerCallback()
{
	// Ray Interest Test
	glm::vec3 start = m_Scene->GetRenderCamera()->GetPosition();
	glm::vec3 end = start + ( Chroma::Input::GetLastRay() * glm::vec3(1000.0));
	IEntity* clickedEntity  = Chroma::Physics::GetEntityRayTest(start, end);
	if (clickedEntity)
		Chroma::GUI::SetSelectedEntityName(clickedEntity->GetName());

}

void ChromaEngine::Tick()
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


ChromaEngine::~ChromaEngine()
{
}

ChromaEngine::ChromaEngine(Scene*& Scene)
{
	m_Scene = Scene;
	Init();
}

void ChromaEngine::Init()
{
	// Time
	Chroma::Time::Init();
	CHROMA_INFO("Chroma Time Initialized.");

	// Renderer
	Chroma::Render::Init(m_Scene);
	CHROMA_INFO("Chroma Renderer Initialized.");

	// Input
	Chroma::Input::Init();
	Chroma::Input::BindCamera(m_Scene->GetRenderCamera());
	Chroma::Input::BindMousePickerCallback(std::bind(&ChromaEngine::MousePickerCallback, this));
	CHROMA_INFO("Chroma Input Initialized.");

	// PhysicsEngine
	Chroma::Physics::Init();
	Chroma::Physics::BindDebugBuffer(Chroma::Render::GetDebugBuffer());
	CHROMA_INFO("Chroma Physics Engine Initialized.");

	// AnimationEngine
	m_Animation->BindScene(m_Scene);
	CHROMA_INFO("Chroma Animation Engine Initialized.");

}

void ChromaEngine::ProcessInput()
{
	// update input
	Chroma::Input::Update();

	// update camera
	if (Chroma::Input::IsPressed(Chroma::Input::NUM1))
		m_Scene->GetRenderCamera()->SetCameraMode(Maya);
	if (Chroma::Input::IsPressed(Chroma::Input::NUM2))
		m_Scene->GetRenderCamera()->SetCameraMode(FlyCam);
	if (Chroma::Input::IsPressed(Chroma::Input::NUM3))
		m_Scene->GetRenderCamera()->SetCameraMode(Custom);

	m_Scene->GetRenderCamera()->Update();

	// render physics debug 
	if (Chroma::GUI::drawPhysicsDebug)
		Chroma::Physics::DrawDebug();

	// animation debug
	//if (Chroma::GUI::DebugAnim)
	Chroma::Render::GetDebugBuffer()->DrawSceneSkeletons(m_Scene);

}
