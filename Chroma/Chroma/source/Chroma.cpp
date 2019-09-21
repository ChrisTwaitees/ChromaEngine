// c++ standard libs
#include <math.h>
#include <iostream>
#include <vector>
#include <map>
// glad and glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// assimp
#include <assimp/Importer.hpp>
// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// local
#include "shaders/Shader.h"
#include "buffers/VertexBuffer.h"
#include "models/Model.h"
#include "models/BoxPrimitive.h"
#include "models/PlanePrimitive.h"
#include "textures/Texture.h"
#include "cameras/Camera.h"
#include "lights/Light.h"
#include "screenManager/ChromaScreenManager.h"



// callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_aim_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processKeyboardInput(GLFWwindow* window, float deltaTime);

// prototypes
void updateLightingUniforms(Shader &shader, std::vector<Light> &lights);
void updateCamera(GLFWwindow* window, float deltaTime);

// perspective camera
Camera MainCamera1;
// projection matrix from camera
glm::mat4 projection_mat = glm::perspective(glm::radians(CAM_FOV), CAM_ASPECT, CAM_NEAR, CAM_FAR);

// time
float deltaTime;
float lastFrame;

int main()
{
	// SCREEN MANAGER
	//ChromaScreenManager* ScreenManager = new ChromaScreenManager;
	//ChromaScreenManager ScreenManager;

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CHROMA", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_aim_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	// Capture mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Initialize GUI
	ChromaGUI ChromaGUI(window);
	
	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);
	// Enable Blending
	glEnable(GL_BLEND);
	// Blending Func
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Enable Face Culling
	glEnable(GL_CULL_FACE);
	

	/*  SCENE ASSEMBLY  */
	// point lights 
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// Dancing cubes
	glm::vec3 cubePositions[] = {
		  glm::vec3(0.0f,  0.0f,  0.0f),
		  glm::vec3(2.0f,  5.0f, -15.0f),
		  glm::vec3(-1.5f, -2.2f, -2.5f),
		  glm::vec3(-3.8f, -2.0f, -12.3f),
		  glm::vec3(2.4f, -0.4f, -3.5f),
		  glm::vec3(-1.7f,  3.0f, -7.5f),
		  glm::vec3(1.3f, -2.0f, -2.5f),
		  glm::vec3(1.5f,  2.0f, -2.5f),
		  glm::vec3(1.5f,  0.2f, -1.5f),
		  glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Grass Positions
	std::vector<glm::vec3> vegetationPositions;
	vegetationPositions.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	vegetationPositions.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	vegetationPositions.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	vegetationPositions.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetationPositions.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	// SHADERS
	Shader lightingShader("resources/shaders/fragLit.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader nanoSuitShader("resources/shaders/fragLit.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader depthShader("resources/shaders/fragDepth.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader constantShader("resources/shaders/fragConstant.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader testShader("resources/shaders/fragTest.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader alphaShader("resources/shaders/fragAlpha.glsl", "resources/shaders/vertexShaderLighting.glsl");

	// TEXTURES
	Texture diffuseMap("resources/textures/wooden_panel.png");
	Texture specularMap("resources/textures/wooden_panel_specular.png");
	Texture grassMap("resources/textures/grass.png");

	// MODELS
	Model NanosuitModel("resources/assets/nanosuit/nanosuit.obj");
	//box primitive
	Mesh *Box = new BoxPrimitive();
	// light primitive
	Mesh *Lamp = new BoxPrimitive();
	// box textures
	Box->bindTexture(diffuseMap);
	Box->bindTexture(specularMap);
	// planes
	Mesh* Plane = new PlanePrimitive();
	Plane->bindTexture(grassMap);

	// LIGHTS
	// dancing point lights
	std::vector<Light> lights;
	for (glm::vec3 pos : pointLightPositions)
	{
		Light pointLight(pos, Light::POINT);
		lights.push_back(pointLight);
	}
	// defailt spot and dir light
	Light sunLight(Light::DIRECTIONAL, glm::vec3(0.2, -0.8, 0.0), 1.0f);
	Light spotLight(Light::SPOT, glm::vec3(0.0f), 0.0f);
	lights.push_back(sunLight);

	// render loop
	
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// Delta time
		float GameTime = glfwGetTime();
		deltaTime = GameTime - lastFrame;
		lastFrame = GameTime;

		// input
		// -----
		processKeyboardInput(window, deltaTime);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		// LIGHTS
		constantShader.use();
		for (int i = 0; i < lights.size(); i++)
		{
			// spin lights
			lights[i].position.x = pointLightPositions[i].x + (std::cos(GameTime * 2.0f + i)) * 4.0f;
			lights[i].position.z = pointLightPositions[i].z + sin(std::sin(GameTime * 2.0f + i)) * 4.0f;
			lights[i].position.y = pointLightPositions[i].y + std::sin(GameTime * 2.5f + i) * 1.0f;
			if (lights[i].type == Light::POINT)
			{
				lights[i].diffuse =glm::mod(lights[i].position, glm::vec3(1.0));
			}
			//vertex
			glm::mat4 model{ 1.0f };
			model = glm::translate(model, lights[i].position);
			model = glm::scale(model, glm::vec3(0.3f));
			constantShader.setMat4("model", model);
			constantShader.setMat4("view", MainCamera1.view);
			constantShader.setMat4("projection", projection_mat);
			// fragment
			constantShader.setVec3("lightColor", lights[i].diffuse);
			constantShader.setFloat("lightIntensity", lights[i].intensity);
			constantShader.setVec3("viewPos", MainCamera1.get_position());
			// draw the lamp
			Lamp->drawScene(constantShader);
		}
		// NANO SUIT

		nanoSuitShader.use();
		glm::mat4 model{ 1.0f };
		model = glm::scale(model, glm::vec3(0.3f));
		nanoSuitShader.setMat4("model", model);
		nanoSuitShader.setMat4("view", MainCamera1.view);
		nanoSuitShader.setMat4("projection", projection_mat);
		updateLightingUniforms(nanoSuitShader, lights);
		nanoSuitShader.setFloat("material.ambientBrightness", 0.06f);
		nanoSuitShader.setFloat("material.roughness", 64.0f);
		nanoSuitShader.setFloat("material.specularIntensity", .65f);
		NanosuitModel.drawScene(nanoSuitShader);

		depthShader.use();
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		depthShader.setMat4("model", model);
		depthShader.setMat4("view", MainCamera1.view);
		depthShader.setMat4("projection", projection_mat);
		NanosuitModel.drawScene(depthShader);
		// vertex
		// Draw Geo
		lightingShader.use(); // don't forget to activate the shader before setting uniforms!  
		// lightingShader uniforms
		lightingShader.setMat4("view", MainCamera1.view);
		lightingShader.setMat4("projection", projection_mat);
		// frag
		lightingShader.setVec3("viewPos", MainCamera1.get_position());
		//// lights
		updateLightingUniforms(lightingShader, lights);
		// materials
		lightingShader.setFloat("material.ambientBrightness", 0.06f);
		lightingShader.setFloat("material.roughness", 32.0f);
		lightingShader.setFloat("material.specularIntensity", 1.0f);
		// CREATING BOXES
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model{ 1.0f };
			model = glm::translate(model, cubePositions[i]);
			float angle = GameTime * ( i + 1 ) * 3.f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);
			Box->drawScene(lightingShader);
		}

		// CREATING GRASS
		alphaShader.use();
		alphaShader.setMat4("projection", projection_mat);
		alphaShader.setMat4("view", MainCamera1.view);

		// Sorting Grass for Transparencey Shading
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < vegetationPositions.size(); i++)
		{
			float distance = glm::length(vegetationPositions[i] - MainCamera1.get_position());
			sorted[distance] = vegetationPositions[i];
		}
		// iterating using map
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			glm::mat4 model{ 1.0f };
			model = translate(model, glm::vec3(it->second));
			testShader.setMat4("model", model);
			Plane->drawScene(alphaShader);
		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	//// ------------------------------------------------------------------------
	delete Box;
	delete Lamp;
	delete Plane;

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processKeyboardInput(GLFWwindow* window, float deltaTime)
{
	// check if should close on this frame
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		return;
	}

	// window capture release mouse
	GLint cursorMode = glfwGetInputMode(window, GLFW_CURSOR);
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	{
		if (cursorMode == GLFW_CURSOR_DISABLED)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			MainCamera1.firstMouse = true;
		}
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	// if mouse caputured update camera
	if (cursorMode == GLFW_CURSOR_DISABLED)
		updateCamera(window, deltaTime);
}

void updateCamera(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		MainCamera1.cameraSpeed = MainCamera1.sprintSpeed * deltaTime;
	}
	else {
		MainCamera1.cameraSpeed = MainCamera1.walkSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		MainCamera1.move(MainCamera1.FORWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		MainCamera1.move(MainCamera1.BACK);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		MainCamera1.move(MainCamera1.RIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		MainCamera1.move(MainCamera1.LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		MainCamera1.move(MainCamera1.UP);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		MainCamera1.move(MainCamera1.DOWN);
	}
}

void mouse_aim_callback(GLFWwindow* window, double xpos, double ypos)
{
	GLint cursorMode = glfwGetInputMode(window, GLFW_CURSOR);
	if (cursorMode == GLFW_CURSOR_DISABLED)
		MainCamera1.processMouseInput(xpos, ypos);
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (CAM_FOV >= 1.0f && CAM_FOV <= 45.0f)
		CAM_FOV -= yoffset;
	CAM_FOV = glm::clamp(CAM_FOV, 1.0f, 45.0f);
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	glViewport(0, 0, width, height);
}

void updateLightingUniforms(Shader& shader, std::vector<Light> &lights)
{
	int pointlights{ 0 };
	int dirlights{ 0 };
	int spotlights{ 0 };
	for (int i = 0; i < lights.size(); i++)
	{
		std::string lightIndex;
		// set uniforms
		switch (lights[i].type) {
		case Light::POINT:
			pointlights++;
			lightIndex = "pointLights[" + std::to_string(pointlights - 1) + "]";
			break;
		case Light::DIRECTIONAL:
			dirlights++;
			lightIndex = "dirLights[" + std::to_string(dirlights - 1) + "]";
			break;
		case Light::SPOT:
			spotlights++;
			lightIndex = "spotLights[" + std::to_string(spotlights - 1) + "]";
			break;
		default:
			break;
		}
		shader.setVec3(lightIndex + ".direction", lights[i].direction);
		shader.setVec3(lightIndex + ".position", lights[i].position);
		shader.setVec3(lightIndex + ".diffuse", lights[i].diffuse);
		shader.setFloat(lightIndex + ".intensity", lights[i].intensity);
		//// lights spotlight
		shader.setFloat(lightIndex + ".spotSize", lights[i].spotSize);
		shader.setFloat(lightIndex + ".penumbraSize", lights[i].penumbraSize);
		//// lights falloff
		shader.setFloat(lightIndex + ".constant", lights[i].constant);
		shader.setFloat(lightIndex + ".linear", lights[i].linear);
		shader.setFloat(lightIndex + ".quadratic", lights[i].quadratic);
	}
}