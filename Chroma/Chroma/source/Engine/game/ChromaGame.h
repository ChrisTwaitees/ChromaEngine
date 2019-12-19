#ifndef _CHROMA_GAME_
#define _CHROMA_GAME_

//stl
#include <iostream>
#include <functional>
// thirdparty
#include <GLFW/glfw3.h>
// chroma
#include <input/Input.h>
#include <renderer/Renderer.h>
#include <scene/Scene.h>
#include <screenmanager/ScreenManager.h>
#include <time/ChromaTime.h>
#include <physics/PhysicsEngine.h>
#include <animation/AnimationEngine.h>

#include <worker/IWorker.h>

class ChromaGame
{
	// functions 
	void Initialize();

	// attrs
	enum State {GAME_ACTIVE, GAME_MENU, GAME_WIN};
	State GameState { GAME_MENU };

	// components
	Scene* m_Scene{ nullptr };
	ScreenManager* m_Screen{ nullptr };
	Renderer* m_Renderer{ nullptr };
	PhysicsEngine* m_Physics{ new PhysicsEngine() };
	AnimationEngine* m_Animation{ new AnimationEngine() };

	// input
	Time m_time;
	Input* m_Input{ new Input };

	// game loop functions
	void ProcessInput();
	void Update();
	void Draw();

	// mouse picker callback
	void MousePickerCallback();

public:
	// Accessors
	double GetGameTime() { return m_time.GetGameTime(); };
	double GetDeltaTime() { return m_time.GetDeltaTime(); };
	State getGameState() const { return GameState; };
	Renderer* getRenderer() const { return m_Renderer; };

	// Game Loop
	void Tick();

	ChromaGame(Scene*& Scene, ScreenManager*& ScreenManager);
	~ChromaGame();

};

#endif