

// chroma
#include <common/PrecompiledHeader.h>
#include <engine/Engine.h>
#include <editor/Editor.h>


#if defined(EDITOR)
int main()
{
	// INIT CHROMA
	Chroma::Engine::Init();

	// INIT EDITOR
	Chroma::Editor::Init();

	// Initialize Scene
	Chroma::Scene::PreSceneBuild();

	// POPULATE SCENE CONTENTS
	Chroma::Editor::Sponza();

	// Init all build components
	Chroma::Scene::PostSceneBuild();

	// RENDER LOOP
	// -----------

	while (Chroma::Screen::IsRunning())
	{
		// PROFILING
		CHROMA_PROFILE_BEGIN_SESSION("Editor");

		// ENGINE TICK
		Chroma::Engine::Tick();
		
		// Physics debug
		if (Chroma::Input::IsPressed(Chroma::Input::P))
		{
			Chroma::Physics::ToggleDrawDebug();
		}

		// EDITOR TICK
		Chroma::Editor::Tick();

		// PROFILING
		CHROMA_PROFILE_END_SESSION();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	Chroma::Screen::Close();
	return 0;
}
#endif