#ifndef _CHROMA_GUI_
#define _CHROMA_GUI_
// imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//stl
#include <iostream>
#include <screenManager/ScreenManager.h>

namespace Chroma
{
	class GUI
	{
		void StartLoop();
		void End();
	public:

		void Init();
		void Draw();

		// GUI Attrs
		// time
		float timeSpeed{ 1.0f };
		// debug
		// render
		bool useSkybox{ true };
		float exposure{ 1.0f };
		float gamma{ 2.2f };
		bool useBloom{ false };
		//debugbuffer
		bool drawDebug{ false };
		bool drawPhysicsDebug{ false };
		// anim
		bool DebugAnim{ false }, drawSkeletonsDebug{ false };
		char AnimClipName[128];
		float DebugAnimClipPos{ 0.0 };
		// graphics
		bool drawGraphicsDebug{ false };
		const char* GraphicsDebugs[8]{ "Alebdo", "Normals", "Metalness", "Roughness", "AO", "SSAO", "Shadows", "Reflections" };
		int graphicsDebugSelected{ 0 };
		// camera
		const char* cameras[2]{ "Maya", "FlyCam" };
		int cameraSelected{ 0 };
		// enities
		std::string SelectedEntity = "No Entity Selected";
		void setSelectedEntityName(std::string newEntity) { SelectedEntity = newEntity; };

		static void ToggleSkybox();
		static void ToggleBloom();
		static void ToggleDebug();
		static void TogglePhysicsDebug();
		static void ToggleAnimationDebug();
		static void ToggleGraphicsDebug();


		GUI() {};
		~GUI() {};
	};
}


#endif