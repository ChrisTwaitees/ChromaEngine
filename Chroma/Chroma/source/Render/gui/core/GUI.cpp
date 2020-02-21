#include "GUI.h"
#include <physics/PhysicsEngine.h>
#include <render/Render.h>
#include <scene/SceneManager.h>

namespace Chroma
{
	// global
	float GUI::timeSpeed;
	std::string GUI::SelectedEntity;
	// render
	bool  GUI::useSkybox;
	float GUI::exposure;
	float GUI::gamma;
	bool  GUI::m_Bloom;
	// debug
	bool GUI::drawPhysicsDebug;
	// anim
	bool GUI::drawAnimMenu;
	bool  GUI::debugAnim, drawSkeletonsDebug;
	char  GUI::animClipName[128];
	float GUI::DebugAnimClipPos;
	// graphics
	bool GUI::drawGraphicsMenu;
	int  GUI::m_GraphicsDebugSelected;
	static const char* GraphicsDebugs[5]{ "Alebdo", "Normals", "MetRoughAO", "SSAO", "Shadows"};
	bool GUI::m_DrawGraphicsDebug;
	// scene manager
	bool GUI::drawSceneManager;
	char GUI::sceneName[128];
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

		// SCENEMANAGER
		drawSceneManager = true;

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

		// Animation 
		if (ImGui::Button("Open Scene Manager Menu"))
			ToggleBool(drawSceneManager);
		if (drawSceneManager)
			DrawSceneManagerMenu();

		// Display Selected Entity
		ImGui::Text("Selected Entity : %s", SelectedEntity.c_str());


		ImGui::End();
	}

	void GUI::DrawGraphicsMenu()
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

	void GUI::DrawSceneManagerMenu()
	{
		ImGui::Begin("Chroma Scene Manager");

		ImGui::InputText("Load Scene Path: ", sceneName, IM_ARRAYSIZE(sceneName));
		// Debug
		if (ImGui::Button("Load Scene"))
			Chroma::SceneManager::LoadScene(sceneName);

		if (ImGui::Button("Load HDR"))
			Chroma::Scene::LoadIBL("resources/textures/ibl/ditchriver_ibl/river_sharp.hdr");


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