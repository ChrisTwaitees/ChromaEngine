#include "UI.h"


namespace Chroma
{
	// locals
	std::vector<void(*)() > UI::m_UICalls;

	// FileBrowser
	std::string UI::m_FileBrowserKey;
	std::string UI::m_FilePathName;
	std::string UI::m_FileDirectory;

	// ColorPicker 
	ImGuiColorEditFlags UI::m_ColorPickerFlags{  ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaPreview };

	void UI::Init()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(Chroma::Screen::GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");

		// Filebrowser
		m_FileBrowserKey = "ChromaFileBrowserKey";
		ImGuiFileDialog::Instance()->SetFilterColor(".json", ImVec4(1, 1, 0, 0.5));

	}

	void UI::Draw()
	{
		Start();

		DrawUICalls();

		End();
	}

	void UI::AddUICall(void(*UICall)())
	{
		m_UICalls.push_back(UICall);
	}


	void UI::OpenFileBrowser(const std::string& fileBrowserName, const char* fileFilters)
	{
		ImGuiFileDialog::Instance()->OpenDialog(m_FileBrowserKey, fileBrowserName.c_str(), fileFilters, ".");
	}

	void UI::DrawFileBrowser()
	{
		if (ImGuiFileDialog::Instance()->FileDialog(m_FileBrowserKey))
		{
			// action if OK
			if (ImGuiFileDialog::Instance()->IsOk == true)
			{
				m_FilePathName = ImGuiFileDialog::Instance()->GetFilepathName();
				m_FileDirectory = ImGuiFileDialog::Instance()->GetCurrentPath();
				// action
				CHROMA_INFO("File selected : {} \nAt Directory : {}", m_FilePathName, m_FileDirectory);
			}
			// close
			ImGuiFileDialog::Instance()->CloseDialog(m_FileBrowserKey);
		}
	}

	void UI::DrawColorPicker(glm::vec3& color)
	{
		ImVec4 tempColor(color.x, color.y, color.z, 1.0);
		ImGui::ColorEdit3("MyColorPickerVec3", (float*)& tempColor, ImGuiColorEditFlags_Float | m_ColorPickerFlags);
		color = glm::vec3(tempColor.x, tempColor.y, tempColor.z);
	}

	void UI::DrawColorPicker(glm::vec4& color)
	{
		ImVec4 tempColor(color.x, color.y, color.z, color.w);
		ImGui::ColorEdit4("MyColorPickerVec4", (float*)& tempColor, ImGuiColorEditFlags_Float | m_ColorPickerFlags);
		color = glm::vec4(tempColor.x, tempColor.y, tempColor.z, tempColor.w);
	}

	void UI::Start()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void UI::End()
	{

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(Chroma::Screen::GetWidthHeight().first, Chroma::Screen::GetWidthHeight().second);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

	}

	void UI::DrawUICalls()
	{
		for (void(*uiCall)()  : m_UICalls)
		{
			uiCall();
		}

		// Clear buffered calls once complete
		m_UICalls.clear();
	}

}