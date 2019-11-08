#include "ChromaGame.h"


void ChromaGame::Update()
{
	//std::cout << "Updating Game" << std::endl;
}

void ChromaGame::Render()
{
	mRenderer->RenderScene();
}

void ChromaGame::MousePickerCallback()
{
	glm::vec3 start = mScene->getRenderCamera()->getPosition();
	glm::vec3 end = start + (  mInputHandler->getLastRayPos() * glm::vec3(10.0));

	// create new line object
	IChromaComponent* newLineMeshComponent = new LinePrimitive(start, end);
	newLineMeshComponent->isLit = false;
	mScene->addNewEntity(newLineMeshComponent);
}

void ChromaGame::Tick()
{
	// update time
	mTime.process();
	// process input
	ProcessInput();

	// update while lag is less than framerate cap
	while (mTime.getLag() >= mTime.getMSPerFrame())
	{
		Update();
		mTime.decreaseLag(mTime.getMSPerFrame());
	}
	// consider sleep if Render misaligning with update https://dewitters.com/dewitters-gameloop/

	// Render Scene
	Render();
}


ChromaGame::~ChromaGame()
{
}

ChromaGame::ChromaGame(ChromaSceneManager*& Scene, ChromaScreenManager*& ScreenManager)
{
	mScene = Scene;
	mScreen = ScreenManager;
	mRenderer = new Renderer(Scene, ScreenManager);
	mInputHandler->bindWindow(ScreenManager->getWindow());
	mInputHandler->bindCamera(Scene->getRenderCamera());
	mInputHandler->bindMousePickerCallback(std::bind(&ChromaGame::MousePickerCallback, this));
}

void ChromaGame::initialize()
{

}

void ChromaGame::ProcessInput()
{
	mInputHandler->setDeltaTime(getDeltaTime());
	mInputHandler->process();
	mScene->RenderCamera->processInput(mInputHandler);
}
