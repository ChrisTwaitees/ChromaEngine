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
#include "../gui/ChromaGUI.h"
// camera
#include "../cameras/Camera.h"
//config
#include  "ChromaScreenManagerConfig.h"



class ChromaScreenManager
{
private:
	// window
	GLFWwindow* window; 

	// camera
	Camera camera;

	// gui
	ChromaGUI gui;

	// time
	float deltaTime{0.0f};
	float lastFrame{0.0f};

	// setup
	int initialize();
	int configureGui();
	int configureRenderer();

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
	void drawScene();
	void drawGUI();

public:
	// getters and setters
	unsigned int getScreenWidth() { return SCREEN_WIDTH; };
	unsigned int getScreenHeight() { return SCREEN_HEIGHT; };
	float getDeltaTime() { return deltaTime; };
	GLFWwindow* getWindow() { return window; };
	Camera getActiveCamera() { return camera; };

	// status
	int shouldClose() { return glfwWindowShouldClose(window); };

	// functions
	void Start();
	void End();
	void Close();

	// construction
	ChromaScreenManager();
	~ChromaScreenManager();
};

#endif
