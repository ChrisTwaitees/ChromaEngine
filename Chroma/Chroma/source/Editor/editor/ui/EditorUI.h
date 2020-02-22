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
		static bool m_Bloom;


	private:
		// functions
		static void DrawPropertiesWindow();
		static void DrawResourceBrowser();
		static void DrawSceneManager();
		static void ParentDockWindow();

		// attrs
		// Scene Manager
		static char m_SceneName[128];

		// GUI Attrs
		// time
		static float timeSpeed;
		// debug
		// render
		static bool useSkybox;
		static float exposure;
		static float gamma;
		//debugbuffer
		static bool drawDebug;
		static bool drawPhysicsDebug;
		// anim
		static bool drawAnimMenu, debugAnim, drawSkeletonsDebug;
		static char animClipName[128];
		static float DebugAnimClipPos;
		// graphics
		static bool drawGraphicsMenu;

		static void ToggleBool(bool& toToggle);

		static std::string SelectedEntity;

		static void DrawMainMenu();

		static void DrawGraphicsMenu();

		static void DrawAnimationMenu();

		static void Draw3DViewport();

		// EDITOR
		// Global Settings
		static bool EditorFullScreen;
		static ImGuiDockNodeFlags EditorRootDockspaceFlags;

		// Viewport
		static ImGuiWindowFlags ViewportWindowFlags;
		static bool EditorViewportOpen;
	};
}



#endif