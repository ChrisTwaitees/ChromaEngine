#include "ChromaGUI.h"

void ChromaGUI::initialize()
{
	// IMGUI GUI Initialization
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void ChromaGUI::Start()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// start
	ImGui::Begin("Chroma Debug");          

}

void ChromaGUI::End()
{
	ImGui::End();
	// Rendering
	ImGui::RenderScene();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ChromaGUI::attachWindow(GLFWwindow& window_val)
{
	window = &window_val;
	initialize();
}

ChromaGUI::ChromaGUI()
{
}

ChromaGUI::ChromaGUI(GLFWwindow* window_val) : window{ window_val }
{
	initialize();
}


ChromaGUI::~ChromaGUI()
{
	initialize();
}
