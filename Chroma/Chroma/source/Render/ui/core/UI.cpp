#include "UI.h"
#include <physics/PhysicsEngine.h>
#include <render/Render.h>
#include <scene/SceneManager.h>

namespace Chroma
{
	// global
	float UI::timeSpeed;
	std::string UI::SelectedEntity;
	// render
	bool  UI::useSkybox;
	float UI::exposure;
	float UI::gamma;
	bool  UI::m_Bloom;
	// debug
	bool UI::drawPhysicsDebug;
	// anim
	bool UI::drawAnimMenu;
	bool  UI::debugAnim, drawSkeletonsDebug;
	char  UI::animClipName[128];
	float UI::DebugAnimClipPos;
	// graphics
	bool UI::drawGraphicsMenu;
	int  UI::m_GraphicsDebugSelected;
	static const char* GraphicsDebugs[5]{ "Alebdo", "Normals", "MetRoughAO", "SSAO", "Shadows"};
	bool UI::m_DrawGraphicsDebug;

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


		// style
		ImGui::StyleColorsClassic();
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(Chroma::Screen::GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");

		// GLOBAL
		timeSpeed = 1.0f;
		// GRAPHICS
		drawGraphicsMenu = false;
		m_GraphicsDebugSelected = 0;
		exposure = 1.0f;
		gamma = 2.2f;
		m_Bloom = false;
		useSkybox = true;
		// GRAPHICS - DEBUG
		drawPhysicsDebug = false;
		m_DrawGraphicsDebug = false;

		// ANIMATION
		// anim
		drawAnimMenu = true;
		debugAnim = false;
		DebugAnimClipPos = 0.0f;

	}

	void UI::Draw()
	{
		Start();

		// Main Menu
		DrawMainMenu();

		End();
	}

	void UI::DrawMainMenu()
	{
		ImGui::Begin("Chroma");

		// Time
		ImGui::Text("Deltatime %.3f ms/frame (%.1f FPS)", Chroma::Time::GetDeltaTime(), Chroma::Time::GetFPS());
		ImGui::SliderFloat("Time Multiply", &timeSpeed, 0.0, 3.0);
		Chroma::Time::SetSpeed(timeSpeed);

		// Enable Graphics Menu
		if (ImGui::Button("Open Graphics Menu"))
			ToggleBool(drawGraphicsMenu);
		if (drawGraphicsMenu)
			DrawGraphicsMenu();

		// Animation 
		if (ImGui::Button("Open Animation Menu"))
			ToggleBool(drawAnimMenu);
		if (drawAnimMenu)
			DrawAnimationMenu();

		// Display Selected Entity
		ImGui::Text("Selected Entity : %s", SelectedEntity.c_str());


		ImGui::End();
	}

	void UI::DrawGraphicsMenu()
	{
		ImGui::Begin("Chroma Graphics");

		// bloom
		ImGui::Checkbox("Bloom", &m_Bloom);

		// debug 
		ImGui::Checkbox("Draw Physics", &drawPhysicsDebug);
		if (drawPhysicsDebug)
			Chroma::Physics::DrawDebug();

		ImGui::Checkbox("Graphics Debug", &m_DrawGraphicsDebug);
		if (m_DrawGraphicsDebug)
		{
			ImGui::ListBox("Graphics Debug Mode", &m_GraphicsDebugSelected, GraphicsDebugs, IM_ARRAYSIZE(GraphicsDebugs));
		}

		ImGui::End();
	}

	void UI::DrawAnimationMenu()
	{
		ImGui::Begin("Chroma Animation");

		// Debug
		if (ImGui::Button("Debug Animation"))
			ToggleBool(debugAnim);

		//if (Chroma::GUI::DebugAnim)
		ImGui::InputText("Animation Clip Name : ", animClipName, IM_ARRAYSIZE(animClipName));
		ImGui::Text("Current Animation : %s", animClipName);
		ImGui::SliderFloat("Animation Clip Position", &DebugAnimClipPos, 0.0, 1.0);
		Chroma::Render::GetDebugBuffer()->DrawSceneSkeletons();

		ImGui::End();
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

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void UI::ToggleBool(bool& toToggle)
	{
		toToggle = toToggle ? false : true;
	}

}