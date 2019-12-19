#include "ChromaGame.h"


void ChromaGame::Update()
{
	// physics
	m_Physics->update(m_time);

	// workers
	IChromaWorker::DoWork();
}

void ChromaGame::Draw()
{
	m_renderer->RenderScene();
}


void ChromaGame::MousePickerCallback()
{
	// Ray Interest Test
	glm::vec3 start = m_scene->GetRenderCamera()->GetPosition();
	glm::vec3 end = start + (  m_input->getLastRay() * glm::vec3(1000.0));
	IChromaEntity* clickedEntity  = m_Physics->rayTest(start, end);
	if (clickedEntity)
		m_screen->setSelectedEntityName(clickedEntity->GetName());

}

void ChromaGame::Tick()
{
	// update time
	m_time.process();
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

ChromaGame::ChromaGame(ChromaScene*& Scene, ChromaScreenManager*& ScreenManager)
{
	m_scene = Scene;
	m_screen = ScreenManager;

	Initialize();

}

void ChromaGame::Initialize()
{
	// input
	m_input->bindWindow(m_screen->getWindow());
	m_input->bindCamera(m_scene->GetRenderCamera());
	m_input->bindMousePickerCallback(std::bind(&ChromaGame::MousePickerCallback, this));

	// renderer
	m_renderer = new Renderer(m_scene, m_screen);

	// add Physics to scene
	m_Physics->bindDebugBuffer(m_renderer->getDebugBuffer());
	m_scene->SetPhysics(m_Physics);
}

void ChromaGame::ProcessInput()
{
	// process input
	m_input->setDeltaTime(getDeltaTime());
	m_input->process(); 

	// update camera
	if (m_screen->cameraSelected == 0)
		m_scene->GetRenderCamera()->SetCameraMode(Maya);
	if (m_screen->cameraSelected == 1)
		m_scene->GetRenderCamera()->SetCameraMode(FlyCam);
	m_scene->GetRenderCamera()->ProcessInput(m_input);

	// render physics debug 
	if (m_screen->drawPhysicsDebug)
		m_Physics->drawDebug();

	// render skeletons debug
	if (m_screen->drawSkeletonsDebug)
		m_renderer->getDebugBuffer()->DrawSceneSkeletons(m_scene);

}
