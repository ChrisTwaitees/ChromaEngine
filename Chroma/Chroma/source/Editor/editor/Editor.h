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
		static void Tick();
	private:
		static void DrawUI();

	};
}


#endif