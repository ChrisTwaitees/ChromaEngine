#ifndef _CHROMA_ENGINE_
#define _CHROMA_ENGINE_

// chroma

#include <screen/Screen.h>
#include <logging/Log.h>
#include <scene/Scene.h>
#include <renderer/Renderer.h>
#include <gui/GUI.h>
#include <physics/PhysicsEngine.h>
#include <animation/AnimationEngine.h>
#include <input/Input.h>

class ChromaEngine
{
	// functions 
	void Init();

	// components
	Scene* m_Scene{ nullptr };
	Renderer* m_Renderer{ nullptr };
	AnimationEngine* m_Animation{ new AnimationEngine() };

	// game loop functions
	void Update();
	void ProcessInput();
	void Draw();

	// mouse picker callback
	void MousePickerCallback();

public:
	// Accessors
	Renderer* GetRenderer() const { return m_Renderer; };

	// Game Loop
	void Tick();

	ChromaEngine(Scene*& Scene);
	~ChromaEngine();

};

#endif