#include "ChromaGame.h"


void ChromaGame::Update()
{
	// physics
	m_physics->update(m_time);
}

void ChromaGame::Draw()
{
	m_renderer->RenderScene();
}


void ChromaGame::MousePickerCallback()
{
	glm::vec3 start = m_scene->getRenderCamera()->getPosition();
	glm::vec3 end = start + (  m_input->getLastRay() * glm::vec3(1000.0));

	// create new line object
	IChromaEntity* newLineEntity = new ChromaEntity;
	m_scene->addEntity(newLineEntity);
	ChromaMeshComponent* newLineMeshComponent = new LinePrimitive(start, end);
	newLineMeshComponent->isLit = false;
	newLineEntity->addComponent(newLineMeshComponent);

	IChromaEntity* clickedEntity  = m_physics->rayTest(start, end);
	if (clickedEntity != NULL)
	{
		std::cout << "Found entity adding velocity" << std::endl;
		for (IChromaComponent* component : clickedEntity->getPhysicsComponents())
			((ChromaPhysicsComponent*)component)->setLinearVelocity(glm::normalize(start - end) * 2.0f);
	}

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
	Draw();
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

	// add Physics to scene
	m_physics->bindDebugBuffer(m_renderer->getDebugBuffer());
	m_scene->setPhysics(m_physics);
}

void ChromaGame::ProcessInput()
{
	// process input
	m_input->setDeltaTime(getDeltaTime());
	m_input->process();
	// update camera
	m_scene->getRenderCamera()->processInput(m_input);
	// render physics debug if triggered
	if (m_screen->drawPhysicsDebug)
		m_physics->drawDebug();
}
