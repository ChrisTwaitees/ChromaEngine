#include "ChromaGame.h"


void ChromaGame::Update()
{
	// Physics
	m_Physics->Update(m_time);

	// Animation
	if (m_Screen->DebugAnim)
		m_Animation->UpdateDebug(m_Screen->AnimClipName, m_Screen->DebugAnimClipPos);
	else
		m_Animation->Update(m_time);
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
	m_time.Process();
	// process input
	ProcessInput();

	// update while lag is less than framerate cap
	while (m_time.GetLag() >= m_time.GetMSPerFrame())
	{
		Update();
		m_time.DecreaseLag(m_time.GetMSPerFrame());
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
	// Input
	m_Input->BindWindow(m_Screen->getWindow());
	m_Input->BindCamera(m_Scene->GetRenderCamera());
	m_Input->BindMousePickerCallback(std::bind(&ChromaGame::MousePickerCallback, this));

	// Renderer
	m_Renderer = new Renderer(m_Scene, m_Screen);

	// PhysicsEngine
	m_Physics->BindDebugBuffer(m_Renderer->GetDebugBuffer());
	m_Scene->SetPhysics(m_Physics);

	// AnimationEngine
	m_Animation->BindScene(m_Scene);

	// Time
	m_Scene->SetTime(&m_time);

}

void ChromaGame::ProcessInput()
{
	// process input
	m_Input->SetDeltaTime(GetDeltaTime());
	m_Input->Process(); 

	// update camera
	if (m_Screen->cameraSelected == 0)
		m_Scene->GetRenderCamera()->SetCameraMode(Maya);
	if (m_Screen->cameraSelected == 1)
		m_Scene->GetRenderCamera()->SetCameraMode(FlyCam);
	m_Scene->GetRenderCamera()->ProcessInput(m_Input);

	// render physics debug 
	if (m_Screen->drawPhysicsDebug)
		m_Physics->drawDebug();

	// animation debug
	if (m_Screen->DebugAnim)
		m_Renderer->GetDebugBuffer()->DrawSceneSkeletons(m_Scene);

}
