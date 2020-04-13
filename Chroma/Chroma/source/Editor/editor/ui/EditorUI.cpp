#include "EditorUI.h"
#include <editor/ui/AnimationEditorUI.h>
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
	ImGuiWindowFlags  EditorUI::m_ViewportWindowFlags;
	bool EditorUI::EditorViewportOpen;
	bool EditorUI::m_IconsVisible;

	int EditorUI::m_IconSize;

	Texture EditorUI::m_LightsIcon;
	Texture EditorUI::m_LightSunIcon;
	Texture EditorUI::m_LightPointIcon;

	int EditorUI::m_ViewportWidth;
	int EditorUI::m_ViewportHeight;
	int EditorUI::m_PrevViewportWidth;
	int EditorUI::m_PrevViewportHeight;

	// World Outliner
	std::string EditorUI::m_SelectedObjectString;
	UID EditorUI::m_SelectedObjectUID;
	bool EditorUI::m_SceneTreeNodeExpanded;

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

		// root
		AddUICall(ParentDockWindow);
		// viewport
		AddUICall(Draw3DViewport);
		Chroma::Render::GetDebugBuffer()->DrawGrid(50, glm::vec3(0.5));
		// content browser
		AddUICall(DrawContentBrowser);
		// world outliner
		AddUICall(DrawWorldOutliner);
		// properties tab
		AddUICall(DrawPropertiesTab);
		// build tab
		AddUICall(DrawBuildTab);
		// entity types
		AddUICall(DrawEntityTypesTab);
		// modes
		AddUICall(DrawEditingModeTab);

		// Draw Icons
		DrawIcons();

		// Draw other editors
		DrawOtherEditorWindows();

		// file browser
		AddUICall(DrawFileBrowser);
	}

	void EditorUI::Init()
	{
		CHROMA_INFO("CHROMA EDITOR :: UI Initialized");
		
		// Editor Options
		EditorFullScreen = true;
		EditorRootDockspaceFlags = ImGuiDockNodeFlags_None;

		m_ViewportWindowFlags |= ImGuiWindowFlags_NoTitleBar;
		//ViewportWindowFlags |= ImGuiWindowFlags_NoMove;
		m_ViewportWindowFlags |= ImGuiWindowFlags_NoScrollbar;

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

		// VIEWPORT
		m_IconsVisible = true;
		m_LightsIcon = Texture("resources/icons/lights_icon.png");
		m_LightSunIcon = Texture("resources/icons/light_sun.png");
		m_LightPointIcon = Texture("resources/icons/light_point.png");
		m_IconSize = 20;

		// WORLD OUTLINER
		m_SceneTreeNodeExpanded = true;

		// OTHER EDITORS
		AnimationEditorUI::Init();
	}

	void EditorUI::ResizeEditorUI(int const& newWidth, int const& newHeight)
	{
		// resize viewport
		ResizeViewport(m_ViewportWidth, m_ViewportHeight);

	}

	void EditorUI::SetSelectedObjectUID(const UID& selectedUID)
	{
		// set UID
		m_SelectedObjectUID = selectedUID;
		// get name for ui
		// attempt to fetch component
		if (Chroma::Scene::GetComponent(m_SelectedObjectUID) != nullptr)
		{
			m_SelectedObjectString = Chroma::Type::GetName(Chroma::Scene::GetComponent(m_SelectedObjectUID)->GetType()) + " : (" + m_SelectedObjectUID.data + ")";
		}

		// attempt to fetch entity
		if (Chroma::Scene::GetEntity(m_SelectedObjectUID) != nullptr)
		{
			m_SelectedObjectString = Chroma::Scene::GetEntity(m_SelectedObjectUID)->GetName() + " : (" + selectedUID.data + ")";
		}
	}

	std::pair<int, int> EditorUI::GetViewportDimensions()
	{
		return std::make_pair(m_ViewportWidth, m_ViewportHeight);
		CHROMA_INFO("Viewport dimensions : {0}, {1}", m_ViewportWidth, m_ViewportHeight);
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
		bool p_open;
		ImGui::Begin("Chroma Editor Docking", &p_open, window_flags);
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
		
			if (ImGui::BeginMenu("File"))
			{

				
				if (ImGui::MenuItem("Open Scene"))
				{
					OpenFileBrowser("Open Scene", ".json\0");
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Save Scene"))
				{
					OpenFileBrowser("Choose File", ".json\0");
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				/*if (ImGui::MenuItem("Flag: NoSplit", "", (EditorRootDockspaceFlags & ImGuiDockNodeFlags_NoSplit) != 0))                 EditorRootDockspaceFlags ^= ImGuiDockNodeFlags_NoSplit;
				if (ImGui::MenuItem("Flag: NoResize", "", (EditorRootDockspaceFlags & ImGuiDockNodeFlags_NoResize) != 0))                EditorRootDockspaceFlags ^= ImGuiDockNodeFlags_NoResize;
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (EditorRootDockspaceFlags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  EditorRootDockspaceFlags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (EditorRootDockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))     EditorRootDockspaceFlags ^= ImGuiDockNodeFlags_PassthruCentralNode;
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (EditorRootDockspaceFlags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          EditorRootDockspaceFlags ^= ImGuiDockNodeFlags_AutoHideTabBar;*/
				
				if (ImGui::MenuItem("Editor Settings"))
				{
					CHROMA_INFO("Opening Editor Settings");
				}

				ImGui::Separator();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();

	}



	void EditorUI::DrawContentBrowser()
	{
		ImGui::Begin("Content Browser");

		ImGui::InputText("Load Scene Path: ", m_SceneName, IM_ARRAYSIZE(m_SceneName));
		// Debug
		if (ImGui::Button("Load Scene"))
			Chroma::SceneManager::LoadScene(m_SceneName);

		if (ImGui::Button("Load HDR"))
			Chroma::Scene::LoadIBL("resources/textures/ibl/ditchriver_ibl/river_sharp.hdr");
		ImGui::End();
	}


	void EditorUI::DrawBuildTab()
	{
		ImGui::Begin("Build");
		ImGui::ImageButton((void*)(intptr_t)Chroma::EditorUI::m_LightsIcon.ID, ImVec2(Chroma::EditorUI::m_IconSize, Chroma::EditorUI::m_IconSize));
		ImGui::End();
	}


	void EditorUI::DrawWorldOutliner()
	{
		ImGui::Begin("World Outliner");

		ImGuiTreeNodeFlags node_flags =   ImGuiTreeNodeFlags_Bullet;

		// Make sure Scene is open from launch
		ImGui::SetNextTreeNodeOpen(m_SceneTreeNodeExpanded);
		// Root Scene
		if (ImGui::TreeNode("Scene"))
		{
			//m_SceneTreeNodeExpanded = ImGui::IsItemToggledOpen();
			ImGui::Separator();

			// Entities
			if (ImGui::TreeNode("Entities"))
			{
				for (std::pair<UID, IEntity*> uidEntity : Chroma::Scene::GetAllEntities())
				{
					std::string EntityNameUIDHeading = uidEntity.second->GetName() + " : (" + uidEntity.first.data + ")";
					
					// Enities Components
					if (ImGui::TreeNodeEx(EntityNameUIDHeading.c_str(), m_SelectedObjectString == EntityNameUIDHeading ? ImGuiTreeNodeFlags_Selected : node_flags))
					{
						// Selecting
						if (ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing())
						{
							m_SelectedObjectString = EntityNameUIDHeading;
							m_SelectedObjectUID = uidEntity.first;
						}

						ImGui::Indent();
						for (UID componentUID : Chroma::Scene::GetEntity(uidEntity.first)->GetComponentUIDs())
						{
							std::string ComponentTypeUID = Chroma::Type::GetName(Chroma::Scene::GetComponent(componentUID)->GetType()) + " : (" + componentUID.data + ")";

							if (ImGui::Selectable(ComponentTypeUID.c_str(), m_SelectedObjectString == ComponentTypeUID))
							{
								// Selecting
								if (ImGui::IsItemClicked)
								{
									m_SelectedObjectUID = componentUID;
									
									m_SelectedObjectString = ComponentTypeUID;
								}
							}
						}
						ImGui::Unindent();
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}

			ImGui::Separator();

			// Components
			if (ImGui::TreeNode("Components"))
			{
				for (std::pair<UID, IComponent*> uidComponent : Chroma::Scene::GetAllComponents())
				{
					std::string ComponentTypeUID = Chroma::Type::GetName(uidComponent.second->GetType()) + " : (" + uidComponent.first.data + ")";
					// Enities Components
					if (ImGui::Selectable(ComponentTypeUID.c_str(), m_SelectedObjectString == ComponentTypeUID))
					{
						// if selected
						if (ImGui::IsItemClicked)
						{
							CHROMA_TRACE("Component : {0} selected.", ComponentTypeUID);
							m_SelectedObjectString = ComponentTypeUID;
							m_SelectedObjectUID = uidComponent.first;
						}
					}
				}
				ImGui::TreePop();
			}

			ImGui::Separator();

			// Lights
			if (ImGui::TreeNode("Lights"))
			{
				for (UID const& lightUID : Chroma::Scene::GetLightUIDs())
				{
					std::string lightTypeName = static_cast<Light*>(Chroma::Scene::GetComponent(lightUID))->GetTypeString();
					std::string LightTypeUID = lightTypeName + " : (" + lightUID.data + ")";

					if (ImGui::Selectable(LightTypeUID.c_str(), m_SelectedObjectString == LightTypeUID ))
					{
						// if selected
						if (ImGui::IsItemClicked)
						{
							CHROMA_TRACE("Component : {0} selected.", LightTypeUID);
							m_SelectedObjectString = LightTypeUID;
							m_SelectedObjectUID = lightUID;
						}
					}
				}
				ImGui::TreePop();
			}

			ImGui::Separator();

			ImGui::TreePop();
		}


		ImGui::End();
	}


	void EditorUI::DrawPropertiesTab()
	{
		ImGui::Begin("Properties");

		ImGui::Text(("Selected Object : " + m_SelectedObjectString).c_str());

		// Get Object Serialization Data
		ISerializer* objectSerializer = new JSONSerializer();

		// attempt to fetch component
		if (Chroma::Scene::GetComponent(m_SelectedObjectUID) != nullptr)
		{
			Chroma::Scene::GetComponent(m_SelectedObjectUID)->Serialize(objectSerializer);
		}

		// attempt to fetch entity
		if (Chroma::Scene::GetEntity(m_SelectedObjectUID) != nullptr)
		{
			Chroma::Scene::GetEntity(m_SelectedObjectUID)->Serialize(objectSerializer);
			Chroma::Scene::GetEntity(m_SelectedObjectUID)->Update();
		}



		// Float Properties
		ImGui::Separator();
		ImGui::Text("Float");
		for (std::pair<const char*, float*> floatProperty : objectSerializer->m_FloatProperties)
		{
			ImGui::SliderFloat(floatProperty.first, floatProperty.second, -10.0, 10.0);
		}

		// Vec3 Properties
		ImGui::Separator();
		ImGui::Text("Vec3");
		for (std::pair<const char*, glm::vec3*> vec3Property : objectSerializer->m_Vec3Properties)
		{
			std::string x = ".x";
			std::string y = ".y";
			std::string z = ".z";
			ImGui::SliderFloat((vec3Property.first + x).c_str(), &vec3Property.second->x, -10.0, 10.0);
			ImGui::SliderFloat((vec3Property.first + y).c_str(), &vec3Property.second->y, -10.0, 10.0);
			ImGui::SliderFloat((vec3Property.first + z).c_str(), &vec3Property.second->z, -10.0, 10.0);
			ImGui::Separator();
			
		}


		delete objectSerializer;
		


		ImGui::End();
	}


	void EditorUI::DrawEntityTypesTab()
	{
		ImGui::Begin("Entity Types");
		ImGui::End();
	}


	void EditorUI::DrawEditingModeTab()
	{
		ImGui::Begin("Editor Mode");
		if (ImGui::Button("Animation Editor"))
			AnimationEditorUI::Open();
		ImGui::End();
	}

	void EditorUI::DrawOtherEditorWindows()
	{
		AnimationEditorUI::Draw();
	}

	void EditorUI::DrawIcons()
	{
#ifdef EDITOR
		if (m_IconsVisible)
		{
			// lights
			for (UID const& lightUID : Chroma::Scene::GetLightUIDs())
			{
				Light* light = static_cast<Light*>(Chroma::Scene::GetComponent(lightUID));
				// set uniforms
				switch (light->type) {
				case Light::POINT:
					light->DrawIcon(m_LightPointIcon);
				case Light::SUNLIGHT:
					light->DrawIcon(m_LightSunIcon);
					break;
				case Light::DIRECTIONAL:
					light->DrawIcon(m_LightSunIcon);
					break;
				case Light::SPOT:
					light->DrawIcon(m_LightPointIcon);
					break;
				default:
					break;
				}
			}
		}
#endif
	}

	void EditorUI::DrawFileBrowser()
	{
		if (ImGuiFileDialog::Instance()->FileDialog(m_FileBrowserKey))
		{
			// action if OK
			if (ImGuiFileDialog::Instance()->IsOk == true)
			{
				m_FilePathName = ImGuiFileDialog::Instance()->GetFilepathName();
				m_FileDirectory = ImGuiFileDialog::Instance()->GetCurrentPath();
				// Load new Scene
				Chroma::SceneManager::LoadScene(m_FilePathName.c_str());
			}
			// close
			ImGuiFileDialog::Instance()->CloseDialog(m_FileBrowserKey);
		}
	}



	void EditorUI::ResizeViewport(int const& newWidth, int const& newHeight)
	{
		Chroma::Render::ResizeBuffers(newWidth, newHeight);
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

		ImGui::Begin("Viewport", &EditorViewportOpen, m_ViewportWindowFlags);
		{
			// Set Dimensions
			m_ViewportWidth = ImGui::GetWindowSize().x;
			m_ViewportHeight = ImGui::GetWindowSize().y;
			// check if resize occurred
			if (m_PrevViewportWidth != m_ViewportWidth || m_PrevViewportHeight != m_ViewportHeight)
			{
				ResizeViewport(m_ViewportWidth, m_ViewportHeight);
			}

			// Set Viewport to  Render Buffer Texture
			ImGui::Image((void*)(intptr_t)Chroma::Render::GetPostFXBuffer()->GetTexture(),
				ImVec2(m_ViewportWidth, m_ViewportHeight),
				ImVec2(0, 1), ImVec2(1, 0));

			// Set prev attrs
			m_PrevViewportHeight = m_ViewportHeight;
			m_PrevViewportWidth = m_ViewportWidth;

		}
		ImGui::End();
	}

	void EditorUI::DrawContentRegionDebug()
	{
		// Debug : Draw rectangle in content region
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();

		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;

		ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, IM_COL32(255, 255, 0, 255));
	}
}