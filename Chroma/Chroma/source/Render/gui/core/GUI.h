#ifndef _CHROMA_GUI_
#define _CHROMA_GUI_
// imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//stl
#include <iostream>

// chroma

#include <common/CoreCommon.h>

namespace Chroma
{
	class GUI
	{
		static void Start();
		static void End();

		static void ToggleBool(bool& toToggle);

		static std::string SelectedEntity;

		static void DrawMainMenu();

		static void DrawGraphicsMenu();

		static void DrawAnimationMenu();

		static void DrawSceneManagerMenu();

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
		static bool drawAnimMenu, debugAnim, drawSkeletonsDebug, drawSceneManager;
		static char animClipName[128];
		static float DebugAnimClipPos;
		// graphics
		static bool drawGraphicsMenu;
		// enities

		// scenes
		static char sceneName[128];

	public:
		// functions
		static void Init();
		static void Draw();

		// graphics
		static bool m_Bloom;
		static bool m_DrawGraphicsDebug;
		static int m_GraphicsDebugSelected;

		inline static void SetSelectedEntityName(std::string const& newEntity) { SelectedEntity = newEntity; };
	};
}


#endif