// c++ standard libs
#include <vector>

// glm
#include <glm/glm.hpp>

// chroma
#include <engine/Engine.h>
#include <editor/Editor.h>
#include <resources/ResourceManager.h>


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

	Chroma::Scene::PostSceneBuild();

	// TEST 
	Json testJson("C:/Users/cthwaites/Desktop/temp/testytest.json");

	// RENDER LOOP
	// -----------
	while (Chroma::Screen::IsRunning())
	{
		// GAME TICK
		Chroma::Engine::Tick();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	Chroma::Screen::Close();
	return 0;
}
#endif