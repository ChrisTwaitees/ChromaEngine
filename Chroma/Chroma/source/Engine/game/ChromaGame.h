#ifndef _CHROMA_GAME_
#define _CHROMA_GAME_

#include <GLFW/glfw3.h>
#include <functional>

#include <iostream>
#include "input/ChromaInput.h"
#include "renderer/Renderer.h"
#include <scene/ChromaScene.h>
#include "screenmanager/ChromaScreenManager.h"
#include "time/ChromaTime.h"
#include "physics/Physics.h"

// testing ray creation
#include "model/LinePrimitive.h"


class ChromaGame
{
	// functions 
	void initialize();

	// attrs
	enum State {GAME_ACTIVE, GAME_MENU, GAME_WIN};
	State GameState { GAME_MENU };

	// components
	ChromaScreenManager* m_screen;
	ChromaScene* m_scene;
	ChromaPhysics* m_physics;
	Renderer* m_renderer;

	// input
	ChromaTime m_time;
	ChromaInput* m_input{ new ChromaInput };

	// game loop functions
	void ProcessInput();
	void Update();
	void Render();

	// mouse picker callback
	void MousePickerCallback();

public:
	double getDeltaTime() { return m_time.getDeltaTime(); };
	State getGameState() { return GameState; };


	// Game Loop
	void Tick();

	ChromaGame(ChromaScene*& Scene, ChromaScreenManager*& ScreenManager);
	~ChromaGame();

};

#endif