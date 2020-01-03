#include "GUI.h"


namespace Chroma
{

	void GUI::Init()
	{
		// IMGUI GUI Initialization
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(Chroma::ScreenManager::GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void GUI::Draw()
	{
		// start
		ImGui::Begin("Chroma Debug");
	}

	void GUI::StartLoop()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GUI::End()
	{
		ImGui::End();
		// Rendering
		ImGui::RenderScene();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}


}