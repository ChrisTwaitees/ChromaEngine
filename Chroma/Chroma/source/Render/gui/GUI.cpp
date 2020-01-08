#include "GUI.h"
#include <physics/PhysicsEngine.h>
#include <render/Render.h>

namespace Chroma
{
	// global
	float GUI::timeSpeed;
	std::string GUI::SelectedEntity;
	// render
	bool  GUI::useSkybox;
	float GUI::exposure;
	float GUI::gamma;
	bool  GUI::bloom;
	// debug
	bool GUI::drawPhysicsDebug;
	// anim
	bool GUI::drawAnimMenu;
	bool  GUI::debugAnim, drawSkeletonsDebug;
	char  GUI::animClipName[128];
	float GUI::DebugAnimClipPos;
	// graphics
	bool GUI::drawGraphicsMenu;
	int  GUI::graphicsDebugSelected;
	static const char* GraphicsDebugs[8]{ "Alebdo", "Normals", "Metalness", "Roughness", "AO", "SSAO", "Shadows", "Reflections" };

	void GUI::Init()
	{
		// context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		// style
		ImGui::StyleColorsClassic();
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(Chroma::Screen::GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");

		// GLOBAL
		timeSpeed = 1.0f;
		// GRAPHICS
		drawGraphicsMenu = false;
		graphicsDebugSelected = 0;
		exposure = 1.0f;
		gamma = 2.2f;
		bloom = true;
		useSkybox = true;
		// GRAPHICS - DEBUG
		drawPhysicsDebug = false;
		// ANIMATION
		// anim
		drawAnimMenu = false;
		debugAnim = false;
		DebugAnimClipPos = 0.0f;
	}

	void GUI::Draw()
	{
		Start();

		// Main Menu
		DrawMainMenu();

		End();
	}

	void GUI::DrawMainMenu()
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

	void GUI::DrawGraphicsMenu()
	{
		ImGui::Begin("Chroma Graphics");

		// bloom
		ImGui::Checkbox("Bloom", &bloom);

		// debug 
		ImGui::Checkbox("Draw Physics", &drawPhysicsDebug);
		if (drawPhysicsDebug)
			Chroma::Physics::DrawDebug();


		ImGui::End();
	}

	void GUI::DrawAnimationMenu()
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

	void GUI::Start()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GUI::End()
	{

		// Rendering
		ImGui::RenderScene();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GUI::ToggleBool(bool& toToggle)
	{
		toToggle = toToggle ? false : true;
	}

}