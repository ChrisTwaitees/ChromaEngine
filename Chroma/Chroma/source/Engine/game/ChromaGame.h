#ifndef _CHROMA_GAME_
#define _CHROMA_GAME_

//stl
#include <iostream>
#include <functional>
// thirdparty
#include <GLFW/glfw3.h>
// chroma
#include <renderer/Renderer.h>
#include <scene/Scene.h>
#include <screenmanager/ScreenManager.h>
#include <physics/PhysicsEngine.h>
#include <animation/AnimationEngine.h>
#include <worker/IWorker.h>
#include <core/Core.h>

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

	// game loop functions
	void ProcessInput();
	void Update();
	void Draw();

	// mouse picker callback
	void MousePickerCallback();

public:
	// Accessors
	Renderer* getRenderer() const { return m_Renderer; };

	// Game Loop
	void Tick();

	ChromaGame(Scene*& Scene, ScreenManager*& ScreenManager);
	~ChromaGame();

};

#endif