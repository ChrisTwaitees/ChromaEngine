// c++ standard libs
#include <math.h>
#include <iostream>
#include <vector>
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
#include "textures/Texture.h"
#include "cameras/Camera.h"
#include "lights/Light.h"

// callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_aim_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, float deltaTime);

// prototypes
void updateLightingUniforms(Shader &shader, std::vector<Light> &lights);

// SETTINGS
// ------------
// SCREEN
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;


// CAMERA
float CAM_FOV{ 45.0f };
const float CAM_ASPECT{ SCR_WIDTH / SCR_HEIGHT };
const float CAM_NEAR{ 0.1f };
const float CAM_FAR{ 100.0f };
// perspective camera
Camera MainCamera;

// time
float deltaTime;
float lastFrame;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	/*  SCENE ASSEMBLY  */
	// point lights 
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// dancing cubes
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

	// SHADERS
	Shader lightingShader("source/shaders/fragLit.glsl", "source/shaders/vertexShaderLighting.glsl");
	Shader nanoSuitShader("source/shaders/fragLit2.glsl", "source/shaders/vertexShaderLighting.glsl");
	Shader constantShader("source/shaders/fragConstant.glsl", "source/shaders/vertexShaderLighting.glsl");

	// TEXTURES
	Texture diffuseMap("source/textures/source/wooden_panel.png");
	Texture specularMap("source/textures/source/wooden_panel_specular.png");

	// MODELS
	Model NanosuitModel("resources/assets/nanosuit/nanosuit.obj");
	//box primitive
	Mesh *Box = new BoxPrimitive();
	// light primitive
	Mesh *Lamp = new BoxPrimitive();
	
	Box->bindTexture(diffuseMap);
	Box->bindTexture(specularMap);

	// LIGHTS
	// dancing point lights
	std::vector<Light> lights;
	for (glm::vec3 pos : pointLightPositions)
	{
		Light pointLight(pos, Light::POINT);
		lights.push_back(pointLight);
	}
	// defailt spot and dir light
	Light sunLight(Light::DIRECTIONAL, glm::vec3(0.2, -0.8, 0.0), 2.0f);
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
		processInput(window, deltaTime);

		// sin
		float sin_fluctuate = (std::sin(GameTime * 2.0f)) * 0.5f + 0.5f;

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// projection matrix from camera
		glm::mat4 projection_mat = glm::perspective(glm::radians(CAM_FOV), CAM_ASPECT, CAM_NEAR, CAM_FAR);

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
			constantShader.setMat4("view", MainCamera.view);
			constantShader.setMat4("projection", projection_mat);
			// fragment
			constantShader.setVec3("lightColor", lights[i].diffuse);
			constantShader.setFloat("lightIntensity", lights[i].intensity);
			constantShader.setVec3("viewPos", MainCamera.get_position());
			// draw the lamp
			Lamp->Draw(constantShader);
		}

		nanoSuitShader.use();
		// lightingShader uniforms
		glm::mat4 model{ 1.0f };
		model = glm::scale(model, glm::vec3(0.2));
		nanoSuitShader.setMat4("model", model);
		nanoSuitShader.setMat4("view", MainCamera.view);
		nanoSuitShader.setMat4("projection", projection_mat);
		updateLightingUniforms(nanoSuitShader, lights);
		NanosuitModel.Draw(nanoSuitShader);
		// vertex
		// Draw Geo
		lightingShader.use(); // don't forget to activate the shader before setting uniforms!  
		// lightingShader uniforms
		lightingShader.setMat4("view", MainCamera.view);
		lightingShader.setMat4("projection", projection_mat);
		// frag
		lightingShader.setVec3("viewPos", MainCamera.get_position());
		//// lights
		updateLightingUniforms(lightingShader, lights);
		// materials
		lightingShader.setFloat("material.ambientBrightness", 0.06f);
		lightingShader.setFloat("material.roughness", 62.0f);
		lightingShader.setFloat("material.specularIntensity", 1.0f);

		// CREATING BOXES
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model{ 1.0f };
			model = glm::translate(model, cubePositions[i]);
			float angle = GameTime * ( i + 1 ) * 3.f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);
			Box->Draw(lightingShader);
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

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		MainCamera.cameraSpeed = MainCamera.sprintSpeed * deltaTime;
	}
	else {
		MainCamera.cameraSpeed = MainCamera.walkSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		MainCamera.move(MainCamera.FORWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		MainCamera.move(MainCamera.BACK);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		MainCamera.move(MainCamera.RIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		MainCamera.move(MainCamera.LEFT);
	}	
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	{
		// Capture mouse
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (CAM_FOV >= 1.0f && CAM_FOV <= 45.0f)
		CAM_FOV -= yoffset;
	CAM_FOV = glm::clamp(CAM_FOV, 1.0f, 45.0f);
}

void mouse_aim_callback(GLFWwindow* window, double xpos, double ypos)
{

	MainCamera.processMouseInput(xpos, ypos);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
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