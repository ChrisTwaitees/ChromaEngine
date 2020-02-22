

// chroma
#include <common/PrecompiledHeader.h>
#include <engine/Engine.h>
#include <editor/Editor.h>
#include <resources/FileHelper.h>


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
	//Chroma::Editor::PopulateTestScene();

	Chroma::Scene::PostSceneBuild();

	std::vector<std::string> files = Chroma::FileHelper::GetFilesFromDirectory("resources/hdri");
	for (std::string const& fileName : files)
	{
		std::cout << fileName << std::endl;
	}

	// TEST 
	Chroma::SceneManager::SaveScene("resources/levels/testSave.json");

	// RENDER LOOP
	// -----------

	while (Chroma::Screen::IsRunning())
	{

		// GAME TICK
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