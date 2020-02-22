#ifndef _CHROMA_EDITOR_UI_H_
#define _CHROMA_EDITOR_UI_H_

#include <UI/core/UI.h>

namespace Chroma
{
	class EditorUI : public UI
	{
	public:
		static void Draw();
		static void Init();
	private:
		// functions
		static void DrawPropertiesWindow();
		static void DrawResourceBrowser();
		static void DrawSceneManager();

		// attrs
		// Scene Manager
		static char m_SceneName[128];
	};
}



#endif