#include "ChromaGame.h"


void ChromaGame::Update()
{
	// physics
	m_physics->update(m_time);
}

void ChromaGame::Render()
{
	m_renderer->RenderScene();
}

void ChromaGame::MousePickerCallback()
{
	glm::vec3 start = m_scene->getRenderCamera()->getPosition();
	glm::vec3 end = start + (  m_input->getLastRayPos() * glm::vec3(10.0));

	// create new line object
	IChromaEntity* newLineEntity = new ChromaEntity;
	ChromaMeshComponent* newLineMeshComponent = new LinePrimitive(start, end);
	newLineMeshComponent->isLit = false;
	newLineEntity->addComponent(newLineMeshComponent);
	m_scene->addEntity(newLineEntity);
}

void ChromaGame::Tick()
{
	// update time
	m_time.process();
	// process input
	ProcessInput();

	// update while lag is less than framerate cap
	while (m_time.getLag() >= m_time.getMSPerFrame())
	{
		Update();
		m_time.decreaseLag(m_time.getMSPerFrame());
	}
	// consider sleep if Render misaligning with update https://dewitters.com/dewitters-gameloop/

	// Render Scene
	Render();
}


ChromaGame::~ChromaGame()
{
}

ChromaGame::ChromaGame(ChromaScene*& Scene, ChromaScreenManager*& ScreenManager)
{
	m_scene = Scene;
	m_screen = ScreenManager;

	initialize();

}

void ChromaGame::initialize()
{
	// input
	m_input->bindWindow(m_screen->getWindow());
	m_input->bindCamera(m_scene->getRenderCamera());
	m_input->bindMousePickerCallback(std::bind(&ChromaGame::MousePickerCallback, this));

	// renderer
	m_renderer = new Renderer(m_scene, m_screen);

	// physics
	m_physics = new ChromaPhysics(m_scene);
}

void ChromaGame::ProcessInput()
{
	m_input->setDeltaTime(getDeltaTime());
	m_input->process();
	m_scene->getRenderCamera()->processInput(m_input);
}
