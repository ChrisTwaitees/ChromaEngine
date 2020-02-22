#include "EditorUI.h"
#include <scene/SceneManager.h>
#include <physics/PhysicsEngine.h>
#include <render/Render.h>
#include <screen/Screen.h>

namespace Chroma
{
	// EDITOR
	// Editor Options
	bool EditorUI::EditorFullScreen;
	ImGuiDockNodeFlags EditorUI::EditorRootDockspaceFlags;

	// Viewport
	ImGuiWindowFlags  EditorUI::ViewportWindowFlags;
	bool EditorUI::EditorViewportOpen;

	// MENUS
	char EditorUI::m_SceneName[128];

	float EditorUI::timeSpeed;
	// debug
	// render
	bool EditorUI::useSkybox;
	float EditorUI::exposure;
	float EditorUI::gamma;
	bool EditorUI::m_Bloom;
	//debugbuffer
	bool EditorUI::drawDebug;
	bool EditorUI::drawPhysicsDebug;
	// anim
	bool EditorUI::drawAnimMenu;
	bool EditorUI::debugAnim;
	bool EditorUI::drawSkeletonsDebug;
	char EditorUI::animClipName[128];
	float EditorUI::DebugAnimClipPos;
	// graphics
	bool EditorUI::drawGraphicsMenu;

	int  EditorUI::m_GraphicsDebugSelected;
	static const char* GraphicsDebugs[5]{ "Alebdo", "Normals", "MetRoughAO", "SSAO", "Shadows" };
	bool EditorUI::m_DrawGraphicsDebug;


	void EditorUI::Draw()
	{
		AddUICall(ParentDockWindow);
		AddUICall(DrawSceneManager);
		AddUICall(DrawAnimationMenu);
		AddUICall(DrawGraphicsMenu);
		AddUICall(DrawMainMenu);
		AddUICall(Draw3DViewport);
	}

	void EditorUI::Init()
	{
		CHROMA_INFO("CHROMA EDITOR :: UI Initialized");
		// Editor Options
		EditorFullScreen = true;
		EditorRootDockspaceFlags = ImGuiDockNodeFlags_None;

		ViewportWindowFlags |= ImGuiWindowFlags_NoTitleBar;
		//ViewportWindowFlags |= ImGuiWindowFlags_NoMove;
		ViewportWindowFlags |= ImGuiWindowFlags_NoScrollbar;

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

	void EditorUI::DrawPropertiesWindow()
	{

	}

	void EditorUI::DrawResourceBrowser()
	{
	}

	void EditorUI::DrawSceneManager()
	{
		ImGui::Begin("Chroma Scene Manager");

		ImGui::InputText("Load Scene Path: ", m_SceneName, IM_ARRAYSIZE(m_SceneName));
		// Debug
		if (ImGui::Button("Load Scene"))
			Chroma::SceneManager::LoadScene(m_SceneName);

		if (ImGui::Button("Load HDR"))
			Chroma::Scene::LoadIBL("resources/textures/ibl/ditchriver_ibl/river_sharp.hdr");

		ImGui::End();
	}
	void EditorUI::ParentDockWindow()
	{
		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (EditorFullScreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (EditorRootDockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		bool* p_open = new bool(true);
		ImGui::Begin("DockSpace Demo", p_open, window_flags);
		ImGui::PopStyleVar();

		if (EditorFullScreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), EditorRootDockspaceFlags);
		}

		// Menu Bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Chroma Editor"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Flag: NoSplit", "", (EditorRootDockspaceFlags & ImGuiDockNodeFlags_NoSplit) != 0))                 EditorRootDockspaceFlags ^= ImGuiDockNodeFlags_NoSplit;
				if (ImGui::MenuItem("Flag: NoResize", "", (EditorRootDockspaceFlags & ImGuiDockNodeFlags_NoResize) != 0))                EditorRootDockspaceFlags ^= ImGuiDockNodeFlags_NoResize;
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (EditorRootDockspaceFlags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  EditorRootDockspaceFlags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (EditorRootDockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))     EditorRootDockspaceFlags ^= ImGuiDockNodeFlags_PassthruCentralNode;
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (EditorRootDockspaceFlags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          EditorRootDockspaceFlags ^= ImGuiDockNodeFlags_AutoHideTabBar;
				ImGui::Separator();
				if (ImGui::MenuItem("Close DockSpace", NULL, false, p_open != NULL))
					* p_open = false;
				ImGui::EndMenu();
			}

			delete p_open;
			ImGui::EndMenuBar();
		}

		ImGui::End();
	}
	void EditorUI::ToggleBool(bool& toToggle)
	{
		toToggle = toToggle ? false : true;
	}

	void EditorUI::DrawMainMenu()
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
		//ImGui::Text("Selected Entity : %s", SelectedEntity.c_str());


		ImGui::End();
	}

	void EditorUI::DrawGraphicsMenu()
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

	void EditorUI::DrawAnimationMenu()
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
	void EditorUI::Draw3DViewport()
	{

		ImGui::Begin("Viewport", &EditorViewportOpen, ViewportWindowFlags);
		{
			ImGui::Image((void*)(intptr_t)Chroma::Render::GetPostFXBuffer()->GetTexture(),
				ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y),
				ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
	}
}