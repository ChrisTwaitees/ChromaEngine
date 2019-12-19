#ifndef _CHROMA_GAME_
#define _CHROMA_GAME_

//stl
#include <iostream>
#include <functional>
// thirdparty
#include <GLFW/glfw3.h>
// chroma
#include <input/ChromaInput.h>
#include <renderer/Renderer.h>
#include <scene/ChromaScene.h>
#include <screenmanager/ChromaScreenManager.h>
#include <time/ChromaTime.h>
#include <physics/PhysicsEngine.h>
#include <worker/IChromaWorker.h>

class ChromaGame
{
	// functions 
	void Initialize();

	// attrs
	enum State {GAME_ACTIVE, GAME_MENU, GAME_WIN};
	State GameState { GAME_MENU };

	// components
	ChromaScene* m_Scene;
	ChromaScreenManager* m_Screen;
	Renderer* m_Renderer;
	PhysicsEngine* m_Physics{ new PhysicsEngine()};

	// input
	ChromaTime m_time;
	ChromaInput* m_Input{ new ChromaInput };

	// game loop functions
	void ProcessInput();
	void Update();
	void Draw();

	// mouse picker callback
	void MousePickerCallback();

public:
	double getDeltaTime() { return m_time.getDeltaTime(); };
	State getGameState() { return GameState; };
	Renderer* getRenderer() { return m_Renderer; };


	// Game Loop
	void Tick();

	ChromaGame(ChromaScene*& Scene, ChromaScreenManager*& ScreenManager);
	~ChromaGame();

};

#endif