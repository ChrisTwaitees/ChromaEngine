#ifndef _CHROMA_EDITOR_UI_H_
#define _CHROMA_EDITOR_UI_H_


#include <UI/core/UI.h>
#include <uid/UID.h>



namespace Chroma
{
	class MouseButtonPressedEvent;
	class IComponent;

	class EditorUI : public UI
	{

	public:
		static void Draw();
		static void Init();
		static void ResizeEditorUI(int const& newWidth, int const& newHeight);

		//event 
		static void OnEvent(Event& e);
		
		static void SetSelectedObjectUID(const UID& selectedUID);
		inline static bool GetIsMouseOverViewport() { return m_MouseIsOverViewport; };
		static glm::vec2 GetViewportMouseCursorCoords();

		static std::pair<int, int> GetViewportDimensions();
		static std::pair<int, int> GetViewportOffset() { return std::make_pair(m_ViewportOffsetX, m_ViewportOffsetY); }

		static bool m_Bloom;
		static bool m_IconsVisible;
		static bool m_VXGI;
		static bool m_VXGIVisualization;


	private:
		//events
		static bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		// Transform Gizmo
		static IComponent* m_TransformGizmo;
		static void UpdateTransformGizmo();

		// functions
		static void ParentDockWindow();
		static void DrawContentBrowser();
		static void DrawBuildTab();
		static void DrawWorldOutliner();
		static void DrawPropertiesTab();
		// EntityTypes
		static void DrawRenderSettingsTab();
		// Editors
		static void DrawEditorsTab();
		static void DrawOtherEditorWindows();
		// viewport
		static void Draw3DViewportTab();
		static void DrawGraphicsDebugMenu();
		static void DrawIcons();
		// utils
		static void DrawFileBrowser();

		// Viewport Functions
		static void ResizeViewport(int const& newWidth, int const& newHeight);

		// attrs
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
		static bool m_DrawGraphicsDebugMenu;
		static bool m_DrawViewportGrid;
		// profiling
		static bool m_DrawProfilerWindow;
		static bool m_ShowProfilerStatsOverlay;

		static void ToggleBool(bool& toToggle);

		static std::string SelectedEntity;

		static void DrawMainMenu();

		static void DrawAnimationMenu();


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
		static int m_ViewportWidth;
		static int m_ViewportHeight;

		static int m_ViewportOffsetX;
		static int m_ViewportOffsetY;

		static int m_IconSize;


		// Lights Icons
		static Texture m_LightsIcon;
		static Texture m_LightSunIcon;
		static Texture m_LightPointIcon;
		static Texture m_ImportIcon;
		static Texture m_TextureIcon;

		// Helpers
		static void DrawContentRegionDebug();


	};
}



#endif