#ifndef _CHROMA_GUI_
#define _CHROMA_GUI_
// imgui
#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>
#include <stdio.h>
#include <imguifiledialog/ImGuiFileDialog.h>

// chroma
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <common/PrecompiledHeader.h>
#include <texture/Texture.h>

namespace Chroma
{
	class UI
	{
	public:
		// functions
		static void Init();
		static void Draw();

		template<typename Fn>
		static void AddUICall(Fn func)
		{
			m_UICalls.push_back(func);
		}

		static void ShowProfilingWindow() { AddUICall(DrawProfilingWindow); };

		// graphics
		static bool m_Bloom;
		static bool m_DrawGraphicsDebug;
		static int m_GraphicsDebugSelected;
		

		inline static void SetSelectedEntityName(std::string const& newEntity) { };

	protected:

		enum FileBrowserMode { kSceneOpen, kSceneSave, kLoadIBL };

		static void OpenFileBrowser(const std::string& fileBrowserName, const char* fileFilters, FileBrowserMode fileBrowserFlags = FileBrowserMode::kSceneOpen);
		static void DrawFileBrowser();
		static void DrawProfilingWindow();

		static void DrawColorPicker(glm::vec3& color);
		static void DrawColorPicker(glm::vec4& color);

		static void Start();
		static void End();
		static void DrawUICalls();


		static std::vector<std::function<void()>> m_UICalls;

		static std::string m_FileBrowserKey;
		static std::string m_FilePathName;
		static std::string m_FileDirectory;

		// color picker
		static ImGuiColorEditFlags m_ColorPickerFlags;
		static FileBrowserMode m_FileBrowserMode;

	};
}


#endif