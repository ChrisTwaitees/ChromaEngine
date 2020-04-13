

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
	Chroma::Editor::PopulateTestScene2();

	Chroma::Scene::PostSceneBuild();

	// TEST 
	Chroma::SceneManager::SaveScene("resources/levels/testSave.json");

	// RENDER LOOP
	// -----------

	while (Chroma::Screen::IsRunning())
	{

		// ENGINE TICK
		Chroma::Engine::Tick();
		
		// EDITOR TICK
		Chroma::Editor::Tick();

	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	Chroma::Screen::Close();
	return 0;
}
#endif