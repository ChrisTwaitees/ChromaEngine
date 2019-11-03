#ifndef _CHROMA_GAME_
#define _CHROMA_GAME_

#include <GLFW/glfw3.h>
#include <iostream>
#include "input/ChromaInput.h"
#include "renderer/Renderer.h"
#include "scene/ChromaSceneManager.h"
#include "screenmanager/ChromaScreenManager.h"
#include "time/ChromaTime.h"


class ChromaGame
{
	// functions 
	void initialize();

	// attrs
	enum State {GAME_ACTIVE, GAME_MENU, GAME_WIN};
	State GameState { GAME_MENU };

	// input
	ChromaTime mTime;
	ChromaInput* mInputHandler{ new ChromaInput};

	// components
	ChromaSceneManager* mScene;
	ChromaScreenManager* mScreen;
	Renderer* mRenderer;

	// game loop functions
	void ProcessInput();
	void Update();
	void Render();

public:
	double getDeltaTime() { return mTime.getDeltaTime(); };
	State getGameState() { return GameState; };


	// Game Loop
	void Tick();

	ChromaGame(ChromaSceneManager*& Scene, ChromaScreenManager*& ScreenManager);
	~ChromaGame();

};

#endif