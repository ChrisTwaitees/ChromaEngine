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

class ChromaScreenManager
{
	// window
	GLFWwindow* window; 

	// gui
	ChromaGUI gui;

	// time
	float delta{0.0f};
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

	// draw screen
	void processTime();
	void drawGUI();

	// dimensions
	int width, height;

public:
	// getters and setters
	unsigned int getScreenWidth() ;
	unsigned int getScreenHeight();
	float getDeltaTime() { return delta; };
	float getTime() { return glfwGetTime(); };

	GLFWwindow* getWindow() { return window; };
	
	// GUI Attrs
	bool useSkybox{ true };
	float exposure{ 1.0f };
	float gamma{ 2.2f };
	bool useBloom{ false };
	bool drawDebug{ false };
	bool drawPhysicsDebug{ false };


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

	// construction
	ChromaScreenManager();
	~ChromaScreenManager();
};

#endif
