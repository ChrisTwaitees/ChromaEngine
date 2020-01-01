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
#include <logging/Log.h>
#include <worker/IWorker.h>

class ChromaGame
{
	// functions 
	void Initialize();

	// components
	Scene* m_Scene{ nullptr };
	ScreenManager* m_Screen{ nullptr };
	Renderer* m_Renderer{ nullptr };
	PhysicsEngine* m_Physics{ new PhysicsEngine() };
	AnimationEngine* m_Animation{ new AnimationEngine() };

	// input
	Input* m_Input{ new Input };

	// time
	Time m_Time;

	// game loop functions
	void ProcessInput();
	void Update();
	void Draw();

	// mouse picker callback
	void MousePickerCallback();

public:
	// Accessors
	double GetGameTime() { return m_Time.GetGameTime(); };
	double GetDeltaTime() { return m_Time.GetDeltaTime(); };
	Renderer* getRenderer() const { return m_Renderer; };

	// Game Loop
	void Tick();

	ChromaGame(Scene*& Scene, ScreenManager*& ScreenManager);
	~ChromaGame();

};

#endif