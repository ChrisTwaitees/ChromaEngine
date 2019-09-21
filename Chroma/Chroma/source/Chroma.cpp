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
#include "buffers/Framebuffer.h"
#include "models/Model.h"
#include "models/BoxPrimitive.h"
#include "models/PlanePrimitive.h"
#include "textures/Texture.h"
#include "cameras/Camera.h"
#include "lights/Light.h"
#include "screenManager/ChromaScreenManager.h"


// prototypes
void updateLightingUniforms(Shader &shader, std::vector<Light> &lights);

int main()
{
	// SCREEN MANAGER
	ChromaScreenManager ScreenManager;
	Camera& ActiveCamera = ScreenManager.getActiveCamera();
	ScreenManager.setUsePostEffects(true);

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
	while (!ScreenManager.shouldClose())
	{
		ScreenManager.Start();
		float GameTime = ScreenManager.getTime();

		// RENDER
		// ------

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
			constantShader.setMat4("view", ActiveCamera.viewMat);
			constantShader.setMat4("projection", ActiveCamera.projectionMat);
			// fragment
			constantShader.setVec3("lightColor", lights[i].diffuse);
			constantShader.setFloat("lightIntensity", lights[i].intensity);
			constantShader.setVec3("viewPos", ActiveCamera.get_position());
			// draw the lamp
			Lamp->drawScene(constantShader);
		}
		// NANO SUIT UNIFORMS
		nanoSuitShader.use();
		glm::mat4 model{ 1.0f };
		model = glm::scale(model, glm::vec3(0.3f));
		nanoSuitShader.setMat4("model", model);
		nanoSuitShader.setMat4("view", ActiveCamera.viewMat);
		nanoSuitShader.setMat4("projection", ActiveCamera.projectionMat);
		updateLightingUniforms(nanoSuitShader, lights);
		nanoSuitShader.setFloat("material.ambientBrightness", 0.06f);
		nanoSuitShader.setFloat("material.roughness", 64.0f);
		nanoSuitShader.setFloat("material.specularIntensity", .65f);
		NanosuitModel.drawScene(nanoSuitShader);

		depthShader.use();
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		depthShader.setMat4("model", model);
		depthShader.setMat4("view", ActiveCamera.viewMat);
		depthShader.setMat4("projection", ActiveCamera.projectionMat);
		NanosuitModel.drawScene(depthShader);

		lightingShader.use(); // don't forget to activate the shader before setting uniforms!  
		// lightingShader uniforms
		lightingShader.setMat4("view", ActiveCamera.viewMat);
		lightingShader.setMat4("projection", ActiveCamera.projectionMat);
		// frag
		lightingShader.setVec3("viewPos", ActiveCamera.get_position());
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
		alphaShader.setMat4("projection", ActiveCamera.projectionMat);
		alphaShader.setMat4("view", ActiveCamera.viewMat);

		// Sorting Grass for Transparencey Shading
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < vegetationPositions.size(); i++)
		{
			float distance = glm::length(vegetationPositions[i] - ActiveCamera.get_position());
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

		ScreenManager.End();

	}

	// optional: de-allocate all resources once they've outlived their purpose:
	//// ------------------------------------------------------------------------
	delete Box;
	delete Lamp;
	delete Plane;

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	ScreenManager.Close();
	return 0;
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (CAM_FOV >= 1.0f && CAM_FOV <= 45.0f)
		CAM_FOV -= yoffset;
	CAM_FOV = glm::clamp(CAM_FOV, 1.0f, 45.0f);
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