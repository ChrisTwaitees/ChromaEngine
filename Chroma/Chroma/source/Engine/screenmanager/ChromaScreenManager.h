#ifndef _CHROMA_SCREENMANAGER_
#define _CHROMA_SCREENMANAGER_
// stl
#include <vector>
#include <iostream>
// glad and glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// glm
#include <glm/glm.hpp>
// ChromaGUI
#include "gui/ChromaGUI.h"
// camera
#include "camera/Camera.h"
//config
#include  "ChromaScreenManagerConfig.h"
// framebuffer
#include "buffer/Framebuffer.h"
// skybox
#include "model/SkyBox.h"

class ChromaScreenManager
{
private:
	// window
	GLFWwindow* window; 

	// camera
	Camera* camera;

	// gui
	ChromaGUI gui;

	// time
	float deltaTime{0.0f};
	float lastFrame{0.0f};

	// setup
	bool initialize();
	bool configureWindow();
	bool configureGui();
	bool configureRenderer();
	bool configureScene();

	// post processing
	bool usePostFX{false};

	// renderer
	static void updateRendererViewportDimensions(int width, int height);

	// callbacks
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_aim_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	// draw screen
	void processInput();
	void updateCamera();
	void processTime();
	void drawGUI();

public:
	// getters and setters
	unsigned int getScreenWidth() { return SCREEN_WIDTH; };
	unsigned int getScreenHeight() { return SCREEN_HEIGHT; };
	float getDeltaTime() { return deltaTime; };
	float getTime() { return glfwGetTime(); };
	GLFWwindow* getWindow() { return window; };
	Camera* getActiveCamera() { return camera; };
	
	// GUI Attrs
	bool useSkybox{true};
	float exposure{ 1.0f };
	float gamma{ 2.2f };
	bool useBloom{ true };


	// status
	int shouldClose() { return glfwWindowShouldClose(window); };

	// functions
	void Start();
	void End();
	void Close();

	void ToggleSkybox();
	void ToggleBloom();

	// construction
	ChromaScreenManager();
	~ChromaScreenManager();
};

#endif
