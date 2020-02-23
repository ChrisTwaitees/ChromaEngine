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

		static int m_IconSize;

	private:
		// functions
		static void ParentDockWindow();
		static void DrawContentBrowser();
		static void DrawBuildTab();
		static void DrawWorldOutliner();
		static void DrawPropertiesTab();
		static void DrawEntityTypesTab();
		static void DrawEditingModeTab();

		static void DrawIcons();

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
		static bool m_IconsVisible;

		// Lights Icons
		static Texture m_LightsIcon;
		static Texture m_LightSunIcon;
		static Texture m_LightPointIcon;
	};
}



#endif