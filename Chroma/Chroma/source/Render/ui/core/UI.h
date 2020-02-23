#ifndef _CHROMA_GUI_
#define _CHROMA_GUI_
// imgui
#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>

// chroma
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <common/CoreCommon.h>
#include <texture/Texture.h>

namespace Chroma
{
	class UI
	{
	public:
		// functions
		static void Init();
		static void Draw();

		static void AddUICall(void (*UICall)());

		// graphics
		static bool m_Bloom;
		static bool m_DrawGraphicsDebug;
		static int m_GraphicsDebugSelected;

		inline static void SetSelectedEntityName(std::string const& newEntity) { };

	protected:
		static void Start();
		static void End();
		static void DrawUICalls();

		static std::vector<void(*)() > m_UICalls;



	};
}


#endif