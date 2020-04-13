#ifndef _CHROMA_EDITOR_UI_H_
#define _CHROMA_EDITOR_UI_H_


#include <UI/core/UI.h>
#include <uid/UID.h>



namespace Chroma
{
	class EditorUI : public UI
	{
	public:
		static void Draw();
		static void Init();
		static void ResizeEditorUI(int const& newWidth, int const& newHeight);
		
		static void SetSelectedObjectUID(const UID& selectedUID);
		inline static bool GetIsMouseOverViewport() { return m_MouseIsOverViewport; };

		static std::pair<int, int> GetViewportDimensions();
		static bool m_Bloom;


	private:

		// functions
		static void ParentDockWindow();
		static void DrawContentBrowser();
		static void DrawBuildTab();
		static void DrawWorldOutliner();
		static void DrawPropertiesTab();
		static void DrawEntityTypesTab();
		static void DrawEditingModeTab();
		static void DrawOtherEditorWindows();

		static void DrawIcons();
		static void DrawFileBrowser();

		// Viewport Functions
		static void ResizeViewport(int const& newWidth, int const& newHeight);

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

		// World Outliner
		static bool m_SceneTreeNodeExpanded;
		static std::string m_SelectedObjectString;
		static UID m_SelectedObjectUID;

		// Viewport
		static ImGuiWindowFlags m_ViewportWindowFlags;
		static bool m_MouseIsOverViewport;
		static bool EditorViewportOpen;
		static bool m_IconsVisible;
		static int m_PrevViewportWidth;
		static int m_PrevViewportHeight;
		static int m_ViewportWidth;
		static int m_ViewportHeight;
		static int m_IconSize;

		// Lights Icons
		static Texture m_LightsIcon;
		static Texture m_LightSunIcon;
		static Texture m_LightPointIcon;

		// Helpers
		static void DrawContentRegionDebug();


	};
}



#endif