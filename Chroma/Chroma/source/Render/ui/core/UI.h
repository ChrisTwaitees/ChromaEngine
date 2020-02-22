#ifndef _CHROMA_GUI_
#define _CHROMA_GUI_
// imgui
#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>

// chroma
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <common/CoreCommon.h>

namespace Chroma
{
	class UI
	{
	public:
		// functions
		static void Init();
		static void Draw();

		// graphics
		static bool m_Bloom;
		static bool m_DrawGraphicsDebug;
		static int m_GraphicsDebugSelected;

		inline static void SetSelectedEntityName(std::string const& newEntity) { SelectedEntity = newEntity; };

	protected:
		static void Start();
		static void End();

		static void ToggleBool(bool& toToggle);

		static std::string SelectedEntity;

		static void DrawMainMenu();

		static void DrawGraphicsMenu();

		static void DrawAnimationMenu();

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


	};
}


#endif