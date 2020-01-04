#include "GUI.h"
#include <physics/PhysicsEngine.h>
#include <render/Render.h>

namespace Chroma
{
	float GUI::timeSpeed;
	// debug
	// render
	bool  GUI::useSkybox;
	float GUI::exposure;
	float GUI::gamma;
	bool  GUI::bloom;
	//debugbuffer
	bool GUI::drawDebug;
	bool GUI::drawPhysicsDebug;
	// anim
	bool  GUI::debugAnim, drawSkeletonsDebug;
	char  GUI::animClipName[128];
	float GUI::DebugAnimClipPos;
	// graphics
	bool GUI::drawGraphicsDebug;
	int  GUI::graphicsDebugSelected;
	// camera
	int GUI::cameraSelected;
	// selected entity
	std::string GUI::SelectedEntity;

	// graphics 
//	static const char* GraphicsDebugs[8]{ "Alebdo", "Normals", "Metalness", "Roughness", "AO", "SSAO", "Shadows", "Reflections" };
//	static const char* cameras[2]{ "Maya", "FlyCam" };

	void GUI::Init()
	{
		// context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		// style
		ImGui::StyleColorsDark();
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(Chroma::Screen::GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");

		// Vars
		timeSpeed = 1.0f;
		useSkybox = true;
		exposure = 1.0f;
		gamma = 2.2f;
		bloom = true;
		//debugbuffer
		drawDebug = false;
		drawPhysicsDebug = false;
		// anim
		debugAnim = false;
		DebugAnimClipPos = 0.0f;
		// graphics
		drawGraphicsDebug = false;
		graphicsDebugSelected = 0;
		// camera
		cameraSelected = 0;
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
		ImGui::Begin("Chroma Debug");

		// Time
		ImGui::Text("Deltatime %.3f ms/frame (%.1f FPS)", Chroma::Time::GetDeltaTime(), Chroma::Time::GetFPS());
		ImGui::SliderFloat("Time Multiply", &timeSpeed, 0.0, 3.0);
		Chroma::Time::SetSpeed(timeSpeed);

		//// exposure
		//ImGui::SliderFloat("Exposure", &exposure, 0.0f, 2.0f);
		//ImGui::SliderFloat("Gamma", &gamma, 0.0f, 5.0f);

		// bloom
		if (ImGui::Button("Toggle Bloom"))
			ToggleBool(bloom);

		//// debug draw
		if (ImGui::Button("Toggle Physics Debug"))
			ToggleBool(drawPhysicsDebug);

		if (ImGui::Button("Toggle Graphics Debug"))
			ToggleBool(drawGraphicsDebug);

		// Graphics Debug Dropdowns 
		//ImGui::Combo("Graphics Debug", &graphicsDebugSelected, GraphicsDebugs, IM_ARRAYSIZE(GraphicsDebugs));

		// camera dropdowns
		//ImGui::Combo("Cameras", &cameraSelected, cameras, IM_ARRAYSIZE(cameras));

		// Animation 
		if (ImGui::Button("Toggle Animation Debug"))
			ToggleBool(debugAnim);

		ImGui::InputText("Animation Clip Name : ", animClipName, IM_ARRAYSIZE(animClipName));
		ImGui::Text("Current Animation : %s", animClipName);
		ImGui::SliderFloat("Animation Clip Position", &DebugAnimClipPos, 0.0, 1.0);
		// Display Selected Entity
		ImGui::Text("Selected Entity : %s", SelectedEntity.c_str());


		// render physics debug 
		if (drawPhysicsDebug)
			Chroma::Physics::DrawDebug();

		// animation debug
		//if (Chroma::GUI::DebugAnim)
		Chroma::Render::GetDebugBuffer()->DrawSceneSkeletons();

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
		ImGui::End();
		// Rendering
		ImGui::RenderScene();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GUI::ToggleBool(bool& toToggle)
	{
		toToggle = toToggle ? false : true;
	}

}