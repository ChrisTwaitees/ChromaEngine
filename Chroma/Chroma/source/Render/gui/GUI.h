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


	public:
		// functions
		static void Init();
		static void Draw();

		// GUI Attrs
		// time
		static float timeSpeed;
		// debug
		// render
		static bool useSkybox;
		static float exposure;
		static float gamma;
		static bool bloom;
		//debugbuffer
		static bool drawDebug;
		static bool drawPhysicsDebug;
		// anim
		static bool drawAnimMenu, debugAnim, drawSkeletonsDebug;
		static char animClipName[128];
		static float DebugAnimClipPos;
		// graphics
		static bool drawGraphicsMenu;
		static int graphicsDebugSelected;
		// enities
		inline static void SetSelectedEntityName(std::string const& newEntity) { SelectedEntity = newEntity; };
	};
}


#endif