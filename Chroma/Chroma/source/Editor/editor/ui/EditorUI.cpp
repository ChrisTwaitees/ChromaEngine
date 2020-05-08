#include "EditorUI.h"
#include <editor/ui/AnimationEditorUI.h>
#include <scene/SceneManager.h>
#include <physics/PhysicsEngine.h>
#include <render/Render.h>
#include <screen/Screen.h>
#include <buffer/GBuffer.h>

namespace Chroma
{
	// EDITOR
	// Editor Options
	bool EditorUI::EditorFullScreen;
	ImGuiDockNodeFlags EditorUI::EditorRootDockspaceFlags;

	// Viewport
	ImGuiWindowFlags  EditorUI::m_ViewportWindowFlags;
	bool EditorUI::m_MouseIsOverViewport;
	bool EditorUI::EditorViewportOpen;
	bool EditorUI::m_IconsVisible;
	bool EditorUI::m_DrawViewportGrid;

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
	bool EditorUI::m_DrawGraphicsDebugMenu;
	// profiling 
	bool EditorUI::m_DrawProfilerWindow;
	bool EditorUI::m_ShowProfilerStatsOverlay;

	int  EditorUI::m_GraphicsDebugSelected;
	static const char* GraphicsDebugs[5]{ "Alebdo", "Normals", "MetRoughAO", "SSAO", "Shadows" };
	bool EditorUI::m_DrawGraphicsDebug;


	void EditorUI::Draw()
	{

		// root
		AddUICall(ParentDockWindow);
		// viewport
		AddUICall(Draw3DViewportTab);
		if(m_DrawViewportGrid)
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
		AddUICall(DrawEditorsTab);

		// Draw Icons
		if(m_IconsVisible)
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
		m_DrawGraphicsDebugMenu = false;
		m_DrawViewportGrid = true;
		m_GraphicsDebugSelected = 0;
		exposure = 1.0f;
		gamma = 2.2f;
		m_Bloom = false;
		useSkybox = true;
		// GRAPHICS - DEBUG
		drawPhysicsDebug = false;
		m_DrawGraphicsDebug = false;

		// PROFILING
		m_DrawProfilerWindow = false;
		m_ShowProfilerStatsOverlay = true;

		// ANIMATION
		// anim
		drawAnimMenu = true;
		debugAnim = false;
		DebugAnimClipPos = 0.0f;

		// VIEWPORT
		m_IconsVisible = true;
		m_MouseIsOverViewport = false;
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

		if (ImGui::Button("SaveScene"))
		{
			Chroma::SceneManager::SaveScene("resources/levels/testSaveScene.json");
			//OpenFileBrowser("Save Scene", ".json\0", UI::FileBrowserMode::kSceneSave);
		}

		// Debug
		if (ImGui::Button("Load Scene"))
		{
			Chroma::SceneManager::LoadScene("resources/levels/testSaveScene.json");
			//OpenFileBrowser("Load Scene", ".json\0", UI::FileBrowserMode::kSceneOpen);
		}

		if (ImGui::Button("Load HDR"))
		{
			OpenFileBrowser("Choose HDR File", ".hdr\0.ibl\0", UI::FileBrowserMode::kLoadIBL);

		}
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
		ISerializer* objectSerializer = FactorySerializer::GetSerializer(Chroma::Type::Serialization::kJSON);

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

		ImGui::Separator();

		// const 
		std::string x = ".x";
		std::string y = ".y";
		std::string z = ".z";
		std::string w = ".w";

		// TransformProperties
		ImGui::Text("Transforms");
		ImGui::Separator();
		for (auto& floatProperty : objectSerializer->m_FloatProperties)
		{
			if (floatProperty.first.m_EditorProperty.m_Type == Chroma::Type::EditorProperty::kTransformProperty)
			{
				ImGui::SliderFloat(floatProperty.first.m_Name, floatProperty.second, floatProperty.first.m_EditorProperty.m_FloatMinMax.first, floatProperty.first.m_EditorProperty.m_FloatMinMax.second);
				ImGui::Separator();
			}
		}

		for (auto& vec3Property : objectSerializer->m_Vec3Properties)
		{
			if (vec3Property.first.m_EditorProperty.m_Type == Chroma::Type::EditorProperty::kTransformProperty)
			{
				std::pair<glm::vec3, glm::vec3> minMax = vec3Property.first.m_EditorProperty.m_Vec3MinMax;
				ImGui::SliderFloat((vec3Property.first.m_Name + x).c_str(), &vec3Property.second->x, minMax.first.x, minMax.second.x);
				ImGui::SliderFloat((vec3Property.first.m_Name + y).c_str(), &vec3Property.second->y, minMax.first.y, minMax.second.y);
				ImGui::SliderFloat((vec3Property.first.m_Name + z).c_str(), &vec3Property.second->z, minMax.first.z, minMax.second.z);
				ImGui::Separator();
			}
		}

		for (auto& vec4Property : objectSerializer->m_Vec4Properties)
		{
			if (vec4Property.first.m_EditorProperty.m_Type == Chroma::Type::EditorProperty::kTransformProperty)
			{
				std::pair<glm::vec4, glm::vec4> minMax = vec4Property.first.m_EditorProperty.m_Vec4MinMax;
				ImGui::SliderFloat((vec4Property.first.m_Name + x).c_str(), &vec4Property.second->x, minMax.first.x, minMax.second.x);
				ImGui::SliderFloat((vec4Property.first.m_Name + y).c_str(), &vec4Property.second->y, minMax.first.y, minMax.second.y);
				ImGui::SliderFloat((vec4Property.first.m_Name + z).c_str(), &vec4Property.second->z, minMax.first.z, minMax.second.z);
				ImGui::SliderFloat((vec4Property.first.m_Name + w).c_str(), &vec4Property.second->w, minMax.first.w, minMax.second.w);
				ImGui::Separator();
			}
		}

		for (auto& quatProperty : objectSerializer->m_QuatProperties)
		{
			if (quatProperty.first.m_EditorProperty.m_Type == Chroma::Type::EditorProperty::kTransformProperty)
			{
				std::pair<glm::vec4, glm::vec4> minMax = quatProperty.first.m_EditorProperty.m_Vec4MinMax;
				ImGui::SliderFloat((quatProperty.first.m_Name + x).c_str(), &quatProperty.second->x, minMax.first.x, minMax.second.x);
				ImGui::SliderFloat((quatProperty.first.m_Name + y).c_str(), &quatProperty.second->y, minMax.first.y, minMax.second.y);
				ImGui::SliderFloat((quatProperty.first.m_Name + z).c_str(), &quatProperty.second->z, minMax.first.z, minMax.second.z);
				ImGui::SliderFloat((quatProperty.first.m_Name + w).c_str(), &quatProperty.second->w, minMax.first.w, minMax.second.w);
				ImGui::Separator();
			}
		}


		// MATERIALS
		ImGui::Text("Materials");
		ImGui::Separator();
		for (auto& floatProperty : objectSerializer->m_FloatProperties)
		{
			if (floatProperty.first.m_EditorProperty.m_Type >= Chroma::Type::EditorProperty::kMaterialProperty && floatProperty.first.m_EditorProperty.m_Type <= Chroma::Type::EditorProperty::kMaterialUniformColorProperty)
			{
				ImGui::SliderFloat(floatProperty.first.m_Name, floatProperty.second, floatProperty.first.m_EditorProperty.m_FloatMinMax.first, floatProperty.first.m_EditorProperty.m_FloatMinMax.second);
				ImGui::Separator();
			}
		}

		for (auto& vec2Property : objectSerializer->m_Vec2Properties)
		{
			if (vec2Property.first.m_EditorProperty.m_Type >= Chroma::Type::EditorProperty::kMaterialProperty && vec2Property.first.m_EditorProperty.m_Type <= Chroma::Type::EditorProperty::kMaterialUniformColorProperty)
			{
				std::pair<glm::vec2, glm::vec2> minMax = vec2Property.first.m_EditorProperty.m_Vec2MinMax;
				ImGui::SliderFloat((vec2Property.first.m_Name + x).c_str(), &vec2Property.second->x, minMax.first.x, minMax.second.x);
				ImGui::SliderFloat((vec2Property.first.m_Name + y).c_str(), &vec2Property.second->y, minMax.first.y, minMax.second.y);
				ImGui::Separator();
			}
		}


		for (auto& vec3Property : objectSerializer->m_Vec3Properties)
		{
			if (vec3Property.first.m_EditorProperty.m_Type >= Chroma::Type::EditorProperty::kMaterialProperty && vec3Property.first.m_EditorProperty.m_Type <= Chroma::Type::EditorProperty::kMaterialUniformColorProperty)
			{
				ImGui::Text(vec3Property.first.m_Name);
				DrawColorPicker(*vec3Property.second);
				ImGui::Separator();
			}
		}

		for (auto& vec4Property : objectSerializer->m_Vec4Properties)
		{
			if (vec4Property.first.m_EditorProperty.m_Type >= Chroma::Type::EditorProperty::kMaterialProperty && vec4Property.first.m_EditorProperty.m_Type <= Chroma::Type::EditorProperty::kMaterialUniformColorProperty)
			{
				ImGui::Text(vec4Property.first.m_Name);
				DrawColorPicker(*vec4Property.second);
				ImGui::Separator();
			}
		}



		// UNCATEGORIZED
		ImGui::Text("Uncategorized");
		ImGui::Separator();

		// Float Properties
		ImGui::Separator();
		ImGui::Text("Float");
		for (auto& floatProperty : objectSerializer->m_FloatProperties)
		{
			if(floatProperty.first.m_EditorProperty.m_Type == Chroma::Type::EditorProperty::kNullEditorProperty)
				ImGui::SliderFloat(floatProperty.first.m_Name, floatProperty.second, floatProperty.first.m_EditorProperty.m_FloatMinMax.first, floatProperty.first.m_EditorProperty.m_FloatMinMax.second);
		}

		// Vec3 Properties
		ImGui::Separator();
		ImGui::Text("Vec3");
		for (auto& vec3Property : objectSerializer->m_Vec3Properties)
		{
			if (vec3Property.first.m_EditorProperty.m_Type == Chroma::Type::EditorProperty::kNullEditorProperty)
			{
				std::pair<glm::vec3, glm::vec3> minMax = vec3Property.first.m_EditorProperty.m_Vec3MinMax;
				ImGui::SliderFloat((vec3Property.first.m_Name + x).c_str(), &vec3Property.second->x, minMax.first.x, minMax.second.x);
				ImGui::SliderFloat((vec3Property.first.m_Name + y).c_str(), &vec3Property.second->y, minMax.first.y, minMax.second.y);
				ImGui::SliderFloat((vec3Property.first.m_Name + z).c_str(), &vec3Property.second->z, minMax.first.z, minMax.second.z);
				ImGui::Separator();
			}
			if (vec3Property.first.m_EditorProperty.m_Type == Chroma::Type::EditorProperty::kColorProperty)
			{
				ImGui::Text(vec3Property.first.m_Name);
				DrawColorPicker(*vec3Property.second);
				ImGui::Separator();
			}
		}

		// Vec4 Properties
		ImGui::Separator();
		ImGui::Text("Vec4");
		for (auto& vec4Property : objectSerializer->m_Vec4Properties)
		{
			if (vec4Property.first.m_EditorProperty.m_Type == Chroma::Type::EditorProperty::kNullEditorProperty)
			{
				std::pair<glm::vec4, glm::vec4> minMax = vec4Property.first.m_EditorProperty.m_Vec4MinMax;
				ImGui::SliderFloat((vec4Property.first.m_Name + x).c_str(), &vec4Property.second->x, minMax.first.x, minMax.second.x);
				ImGui::SliderFloat((vec4Property.first.m_Name + y).c_str(), &vec4Property.second->y, minMax.first.y, minMax.second.y);
				ImGui::SliderFloat((vec4Property.first.m_Name + z).c_str(), &vec4Property.second->z, minMax.first.z, minMax.second.z);
				ImGui::SliderFloat((vec4Property.first.m_Name + w).c_str(), &vec4Property.second->w, minMax.first.w, minMax.second.w);
				ImGui::Separator();
			}
		}

		// Quat Properties
		ImGui::Separator();
		ImGui::Text("Quat");
		for (auto& quatProperty : objectSerializer->m_QuatProperties)
		{
			if (quatProperty.first.m_EditorProperty.m_Type == Chroma::Type::EditorProperty::kNullEditorProperty)
			{
				std::pair<glm::vec4, glm::vec4> minMax = quatProperty.first.m_EditorProperty.m_Vec4MinMax;
				ImGui::SliderFloat((quatProperty.first.m_Name + x).c_str(), &quatProperty.second->x, minMax.first.x, minMax.second.x);
				ImGui::SliderFloat((quatProperty.first.m_Name + y).c_str(), &quatProperty.second->y, minMax.first.y, minMax.second.y);
				ImGui::SliderFloat((quatProperty.first.m_Name + z).c_str(), &quatProperty.second->z, minMax.first.z, minMax.second.z);
				ImGui::SliderFloat((quatProperty.first.m_Name + w).c_str(), &quatProperty.second->w, minMax.first.w, minMax.second.w);
				ImGui::Separator();
			}
		}


		delete objectSerializer;
		


		ImGui::End();
	}


	void EditorUI::DrawEntityTypesTab()
	{
		ImGui::Begin("Entity Types");
		ImGui::End();
	}


	void EditorUI::DrawEditorsTab()
	{
		ImGui::Begin("Editors");
		if (ImGui::Button("Animation Editor"))
			AnimationEditorUI::Open();

		ImGui::End();
	}

	void EditorUI::DrawOtherEditorWindows()
	{
		AnimationEditorUI::Draw();

		if (m_DrawGraphicsDebugMenu)
			AddUICall(DrawGraphicsDebugMenu);

		if (m_DrawProfilerWindow)
			AddUICall(DrawProfilingWindow);
	}

	void EditorUI::DrawIcons()
	{
#ifdef EDITOR

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
#endif


	}

	void EditorUI::DrawProfilingWindow()
	{
		ImGui::Begin("Profiling Tools");

		ImGui::End();
	}

	void EditorUI::DrawFileBrowser()
	{
		if (ImGuiFileDialog::Instance()->FileDialog(m_FileBrowserKey))
		{
			// action if OK
			if (ImGuiFileDialog::Instance()->IsOk)
			{
				m_FilePathName = ImGuiFileDialog::Instance()->GetFilepathName();
				m_FileDirectory = ImGuiFileDialog::Instance()->GetCurrentPath();
				// Load new Scene
				switch (m_FileBrowserMode)
				{
				case(UI::FileBrowserMode::kSceneOpen):
					{
						Chroma::SceneManager::LoadScene(m_FilePathName.c_str());
						break;
					}
				case(UI::FileBrowserMode::kSceneSave):
				{
					Chroma::SceneManager::SaveScene(m_FilePathName.c_str());
					break;
				}
				case(UI::FileBrowserMode::kLoadIBL):
				{
					Chroma::Scene::LoadIBL(m_FilePathName);
					break;
				}
				default :
				{
					CHROMA_ERROR("CHROMA EDITOR :: File Browser Mode Node Supported.");
				}
				}

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
			ToggleBool(m_DrawGraphicsDebugMenu);
		if (m_DrawGraphicsDebugMenu)
			DrawGraphicsDebugMenu();

		// Animation 
		if (ImGui::Button("Open Animation Menu"))
			ToggleBool(drawAnimMenu);
		if (drawAnimMenu)
			DrawAnimationMenu();

		// Display Selected Entity
		//ImGui::Text("Selected Entity : %s", SelectedEntity.c_str());


		ImGui::End();
	}

	void EditorUI::DrawGraphicsDebugMenu()
	{
		ImGui::Begin("Chroma Graphics Debug");
		float debugScale = 0.25;

		// Albedo
		ImGui::BeginChild("Albedo", ImVec2((float)m_ViewportWidth * debugScale, (float)m_ViewportHeight * debugScale ), true);
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImGui::Image((void*)(intptr_t)Chroma::Render::GetAlbedo(),
			ImGui::GetWindowSize(),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
		ImGui::BeginChild("Albedo");
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x + 10 , p.y + 10), IM_COL32(255, 255, 255, 255), "Albedo", NULL, 0.0f);
		ImGui::EndChild();

		// Normals
		ImGui::BeginChild("Normals", ImVec2((float)m_ViewportWidth * debugScale, (float)m_ViewportHeight * debugScale), true);
		p = ImGui::GetCursorScreenPos();
		ImGui::Image((void*)(intptr_t)Chroma::Render::GetWSNormals(),
			ImGui::GetWindowSize(),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
		ImGui::BeginChild("Normals");
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x + 10, p.y + 10), IM_COL32(255, 255, 255, 255), "Normals", NULL, 0.0f);
		ImGui::EndChild();

		// MetRoughAO
		ImGui::BeginChild("MetRoughAO", ImVec2((float)m_ViewportWidth * debugScale, (float)m_ViewportHeight * debugScale), true);
		p = ImGui::GetCursorScreenPos();
		ImGui::Image((void*)(intptr_t)Chroma::Render::GetMetRoughAO(),
			ImGui::GetWindowSize(),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
		ImGui::BeginChild("MetRoughAO");
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x + 10, p.y + 10), IM_COL32(255, 255, 255, 255), "Metallness Roughness AO", NULL, 0.0f);
		ImGui::EndChild();

		// SSAO
		ImGui::BeginChild("SSAO", ImVec2((float)m_ViewportWidth * debugScale, (float)m_ViewportHeight * debugScale), true);
		p = ImGui::GetCursorScreenPos();
		ImGui::Image((void*)(intptr_t)static_cast<GBuffer*>(Chroma::Render::GetGBuffer())->GetSSAOTexture(),
			ImGui::GetWindowSize(),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
		ImGui::BeginChild("SSAO");
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x + 10, p.y + 10), IM_COL32(255, 255, 255, 255), "SSAO", NULL, 0.0f);
		ImGui::EndChild();

		// Positions
		ImGui::BeginChild("Positions", ImVec2((float)m_ViewportWidth * debugScale, (float)m_ViewportHeight * debugScale), true);
		p = ImGui::GetCursorScreenPos();
		ImGui::Image((void*)(intptr_t)Chroma::Render::GetWSPositions(),
			ImGui::GetWindowSize(),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
		ImGui::BeginChild("Positions");
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x + 10, p.y + 10), IM_COL32(255, 255, 255, 255), "WS Positions", NULL, 0.0f);
		ImGui::EndChild();

		// SunlightShadowMap
		ImGui::BeginChild("SunlightShadowMap", ImVec2((float)m_ViewportWidth * debugScale, (float)m_ViewportHeight * debugScale), true);
		p = ImGui::GetCursorScreenPos();
		ImGui::Image((void*)(intptr_t)static_cast<ShadowBuffer*>(Chroma::Render::GetShadowBuffer())->GetTexture(),
			ImGui::GetWindowSize(),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
		ImGui::BeginChild("SunlightShadowMap");
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x + 10, p.y + 10), IM_COL32(255, 255, 255, 255), "Sunlight ShadowMap", NULL, 0.0f);
		ImGui::EndChild();

		// ForwardBuffer
		ImGui::BeginChild("ForwardBuffer", ImVec2((float)m_ViewportWidth * debugScale, (float)m_ViewportHeight * debugScale), true);
		p = ImGui::GetCursorScreenPos();
		ImGui::Image((void*)(intptr_t)Chroma::Render::GetForwardBuffer()->GetTexture(),
			ImGui::GetWindowSize(),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
		ImGui::BeginChild("ForwardBuffer");
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x + 10, p.y + 10), IM_COL32(255, 255, 255, 255), "ForwardBuffer", NULL, 0.0f);
		ImGui::EndChild();


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
	void EditorUI::Draw3DViewportTab()
	{

		ImGui::Begin("Viewport", &EditorViewportOpen, m_ViewportWindowFlags);
		{
			// Settings
			// Graphics
			ImGui::Checkbox("Draw Icons", &m_IconsVisible); ImGui::SameLine();
			ImGui::Checkbox("Draw Grid", &m_DrawViewportGrid); ImGui::SameLine();
			if (ImGui::Button("Graphics Debug")) m_DrawGraphicsDebugMenu = m_DrawGraphicsDebugMenu ? false : true; ImGui::SameLine();
			//Profiling
			ImGui::Checkbox("Profiling Stats", &m_ShowProfilerStatsOverlay); ImGui::SameLine();
			if (ImGui::Button("Profiling Window"))	ToggleBool(m_DrawProfilerWindow);

			// Check if mouse hovering
			m_MouseIsOverViewport = ImGui::IsWindowHovered();

			// Set Dimensions
			m_ViewportWidth = ImGui::GetWindowSize().x;
			m_ViewportHeight = ImGui::GetWindowSize().y;
			// check if resize occurred
			if (m_PrevViewportWidth != m_ViewportWidth || m_PrevViewportHeight != m_ViewportHeight)
			{
				ResizeViewport(m_ViewportWidth, m_ViewportHeight);
			}

			// Set Viewport to  Render Buffer Texture
			ImGui::BeginChild("Main3dViewport", ImVec2((float)m_ViewportWidth , (float)m_ViewportHeight), true);
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImGui::Image((void*)(intptr_t)Chroma::Render::GetPostFXBuffer()->GetTexture(),
				ImVec2(m_ViewportWidth, m_ViewportHeight),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::EndChild();
			// Profiler Stats
			if (m_ShowProfilerStatsOverlay)
			{
				ImGui::BeginChild("Main3dViewport");
				std::vector<std::string> debugStrings;

				// FPS
				std::string FPS("FPS : ");
				FPS.append(std::to_string(1.0 / DELTATIME));
				debugStrings.push_back(FPS);

				// DeltaTime
				std::string DeltaTime("Delta Time : ");
				DeltaTime.append(std::to_string(DELTATIME));
				debugStrings.push_back(DeltaTime);
				// Game Time
				std::string GameTime("Game Time : ");
				GameTime.append(std::to_string(GAMETIME));
				debugStrings.push_back(GameTime);
				
				for (unsigned int i =0 ; i < debugStrings.size() ; i ++)
				{
					ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 1.25, ImVec2(p.x + 10, p.y + (i* ImGui::GetFontSize() * 1.25 + 5)), IM_COL32(255, 255, 255, 255), debugStrings[i].c_str(), NULL, 0.0f);

				}
				ImGui::EndChild();
			}





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