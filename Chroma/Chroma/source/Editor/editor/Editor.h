#ifndef _CHROMA_EDITOR_H_
#define _CHROMA_EDITOR_H_

#include <common/CoreCommon.h>
#include <editor/ui/EditorUI.h>

namespace Chroma
{
	class Editor
	{
	public:
		static void Init();
		static void PopulateTestScene();
		static void PopulateTestScene2();
		static void PopulateTestScene3();
		static void Test(std::vector<Texture*>& textures);

		// Test Scenes
		static void CornellBox();
		static void Sponza();
		static void Dragon();
		static void Buddha();
		static void StanfordBunny();

		static void Tick();
	private:
		static void DrawUI();

	};
}


#endif