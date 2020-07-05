#ifndef _CHROMA_ENGINE_
#define _CHROMA_ENGINE_

// chroma
#include <common/CoreCommon.h>
#include <scene/Scene.h>
#include <scene/SceneManager.h>
#include <render/Render.h>
#include <UI/core/UI.h>
#include <physics/PhysicsEngine.h>
#include <input/Input.h>
#include <jobSystem/JobSystem.h>


namespace Chroma
{
	class Engine
	{
		// game loop functions
		static void UpdateComponents();
		static void Draw();

	public:
		static void Init();
		// Game Loop
		static void OnUpdate();
	};
}



#endif