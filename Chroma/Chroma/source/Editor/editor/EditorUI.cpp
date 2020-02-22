#include "EditorUI.h"
#include <scene/SceneManager.h>

namespace Chroma
{
	char EditorUI::m_SceneName[128];

	void EditorUI::Draw()
	{
		// start frame
		//Start();

		// Properties
		//DrawPropertiesWindow();

		// ResourceBrowser
		//DrawResourceBrowser();

		// SceneManager
		//DrawSceneManager();

		// end frame
		//End();
	}
	void EditorUI::Init()
	{
		CHROMA_INFO("CHROMA EDITOR :: UI Initialized");

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
}