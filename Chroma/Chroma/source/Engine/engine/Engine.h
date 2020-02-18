#ifndef _CHROMA_ENGINE_
#define _CHROMA_ENGINE_

// chroma
#include <common/CoreCommon.h>
#include <core/Core.h>
#include <screen/Screen.h>
#include <scene/Scene.h>
#include <render/Render.h>
#include <gui/core/GUI.h>
#include <physics/PhysicsEngine.h>
#include <input/Input.h>
#include <jobSystem/JobSystem.h>


namespace Chroma
{
	class Engine
	{
		// game loop functions
		static void Update();
		static void UpdateComponents();
		static void ProcessInput();
		static void Draw();

	public:
		static void Init();
		// Game Loop
		static void Tick();
	};
}



#endif