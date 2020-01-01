#include "ChromaGame.h"


void ChromaGame::Update()
{
	// Physics
	m_Physics->Update(m_Time);

	// Animation
	if (m_Screen->DebugAnim)
		m_Animation->UpdateDebug(m_Screen->AnimClipName, m_Screen->DebugAnimClipPos);
	else
		m_Animation->Update(m_Time);

	// Workers
	IWorker::DoWork();
}

void ChromaGame::Draw()
{
	m_Renderer->RenderScene();
}


void ChromaGame::MousePickerCallback()
{
	// Ray Interest Test
	glm::vec3 start = m_Scene->GetRenderCamera()->GetPosition();
	glm::vec3 end = start + (  m_Input->GetLastRay() * glm::vec3(1000.0));
	IEntity* clickedEntity  = m_Physics->GetEntityRayTest(start, end);
	if (clickedEntity)
		m_Screen->setSelectedEntityName(clickedEntity->GetName());

}

void ChromaGame::Tick()
{
	// update time
	m_Time.Process();
	// process input
	ProcessInput();

	// update while lag is less than framerate cap
	while (m_Time.GetLag() >= m_Time.GetMSPerFrame())
	{
		Update();
		m_Time.DecreaseLag(m_Time.GetMSPerFrame());
	}
	// consider sleep if Render misaligning with update https://dewitters.com/dewitters-gameloop/

	// Render Scene
	Draw();
}


ChromaGame::~ChromaGame()
{
}

ChromaGame::ChromaGame(Scene*& Scene, ScreenManager*& ScreenManager)
{
	m_Scene = Scene;
	m_Screen = ScreenManager;

	Initialize();

}

void ChromaGame::Initialize()
{
	// Logging
	Chroma::Log::Init();
	CHROMA_CORE_INFO("Chroma Initializing...");

	// Input
	m_Input->BindWindow(m_Screen->GetWindow());
	m_Input->BindCamera(m_Scene->GetRenderCamera());
	m_Input->BindMousePickerCallback(std::bind(&ChromaGame::MousePickerCallback, this));
	CHROMA_CORE_INFO("Chroma Input Bound.");

	// Renderer
	m_Renderer = new Renderer(m_Scene, m_Screen);
	CHROMA_CORE_INFO("Chroma Renderer Initialized.");

	// PhysicsEngine
	m_Physics->BindDebugBuffer(m_Renderer->GetDebugBuffer());
	m_Scene->SetPhysics(m_Physics);
	CHROMA_CORE_INFO("Chroma Physics Engine Initialized.");

	// AnimationEngine
	m_Animation->BindScene(m_Scene);
	CHROMA_CORE_INFO("Chroma Animation Engine Initialized.");

	// Time
	m_Scene->SetTime(&m_Time);

}

void ChromaGame::ProcessInput()
{
	// process input
	m_Input->SetDeltaTime(GetDeltaTime());
	m_Input->Process(); 

	// update camera
	if (m_Screen->cameraSelected == 0)
		m_Scene->GetRenderCamera()->SetCameraMode(FlyCam);
	if (m_Screen->cameraSelected == 1)
		m_Scene->GetRenderCamera()->SetCameraMode(Maya);
	m_Scene->GetRenderCamera()->ProcessInput(m_Input);

	// render physics debug 
	if (m_Screen->drawPhysicsDebug)
		m_Physics->drawDebug();

	// animation debug
	//if (m_Screen->DebugAnim)
	m_Renderer->GetDebugBuffer()->DrawSceneSkeletons(m_Scene);

}
