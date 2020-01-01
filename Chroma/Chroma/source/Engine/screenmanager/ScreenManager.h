#ifndef _CHROMA_SCREENMANAGER_H_
#define _CHROMA_SCREENMANAGER_H_
// stl
#include <vector>
#include <iostream>
// glad and glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// glm
#include <glm/glm.hpp>
// ChromaGUI
#include <gui/ChromaGUI.h>
// camera
#include <camera/Camera.h>
//config
#include  <ChromaConfig.h>
// framebuffer
#include "buffer/Framebuffer.h"
#include <core/Core.h>

class ScreenManager
{
	// window
	GLFWwindow* window; 

	// gui
	ChromaGUI gui;

	// time
	float m_Delta{0.0f};
	float lastFrame{0.0f};

	// setup
	bool Initialize();
	bool configureWindow();
	bool configureGui();
	bool configureRenderer();

	// renderer
	static void updateRendererViewportDimensions(int width, int height);

	// callbacks
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	// draw screen
	void processTime();
	void drawGUI();

	// dimensions
	int width, height;

public:
	// getters and setters
	unsigned int getScreenWidth() ;
	unsigned int getScreenHeight();
	std::pair<int, int> getWidthHeight();
	float GetDeltaTime() { return m_Delta; };
	float getTime() { return glfwGetTime(); };

	GLFWwindow* GetWindow() { return window; };
	
	// GUI Attrs
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
	bool DebugAnim{ false }, drawSkeletonsDebug{false};
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
	// animation



	// status
	int shouldClose() { return glfwWindowShouldClose(window); };

	// functions
	void StartLoop();
	void EndLoop();
	void Close();

	void ToggleSkybox();
	void ToggleBloom();
	void ToggleDebug();
	void TogglePhysicsDebug();
	void ToggleAnimationDebug();
	void ToggleGraphicsDebug();

	// construction
	ScreenManager();
	~ScreenManager();
};

#endif
