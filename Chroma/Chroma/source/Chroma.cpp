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
// Chroma
#include "screenManager/ChromaScreenManager.h"
#include "entity/Entity.h"
#include "buffers/VertexBuffer.h"
#include "buffers/Framebuffer.h"
#include "buffers/ShadowBuffer.h"
#include "models/Model.h"
#include "models/BoxPrimitive.h"
#include "models/PlanePrimitive.h"
#include "models/SkyBox.h"
#include "shaders/Shader.h"
#include "textures/Texture.h"
#include "cameras/Camera.h"
#include "lights/Light.h"
#include "terrain/Terrain.h"


// prototypes
void updateLightingUniforms(Shader &shader, const std::vector<Light> &lights, Camera& camera);
void renderScene(const Shader& shader);

int main()
{
	// SCREEN MANAGER
	ChromaScreenManager ScreenManager;
	Camera& MainCamera = ScreenManager.getActiveCamera();

	// SCENE MANAGER
	std::vector<Entity> Entities;

	// Shadowbuffer
	ShadowBuffer Shadowbuffer;

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
	Shader litReflectShader("resources/shaders/fragLitReflect.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader refractionShader("resources/shaders/fragRefraction.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader depthShader("resources/shaders/fragDepth.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader constantShader("resources/shaders/fragConstant.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader testShader("resources/shaders/fragTest.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader alphaShader("resources/shaders/fragAlpha.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader debugNormalsShader("resources/shaders/fragDebugNormals.glsl", "resources/shaders/vertexDebugNormals.glsl", "resources/shaders/geometryDebugNormals.glsl");
	bool debugNormals{false};

	// TEXTURES
	Texture diffuseMap("resources/textures/wooden_panel.png");
	Texture specularMap("resources/textures/wooden_panel_specular.png");
	Texture grassMap("resources/textures/grass.png");
	Texture terrainTex("resources/textures/terrain1.jpeg");

	// LIGHTS
// dancing point lights
	std::vector<Light> Lights;
	for (glm::vec3 pos : pointLightPositions)
	{
		Light pointLight(pos, Light::POINT);
		Lights.push_back(pointLight);
	}
	// default spot and dir light
	Light sunLight(Light::DIRECTIONAL, glm::vec3(0.2, -0.8, 0.0), 1.75f);
	Light spotLight(Light::SPOT, glm::vec3(0.0f), 0.0f);
	Lights.push_back(sunLight);


	// ENTITIES
	Entity* NanosuitModel = new Model("resources/assets/nanosuit/nanosuit.obj");
	Entity* Box = new BoxPrimitive();
	Box->bindTexture(diffuseMap);
	Box->bindTexture(specularMap);
	Mesh *Lamp = new BoxPrimitive();
	Mesh *Plane = new PlanePrimitive();
	Plane->bindTexture(grassMap);
	Entity *pTerrain = new Terrain;
	pTerrain->bindCamera(&MainCamera);
	pTerrain->bindLights(&Lights);



	// RENDER LOOP
	
	// -----------
	while (!ScreenManager.shouldClose())
	{
		// SCREENMANAGER START
		ScreenManager.Start();
		float GameTime = ScreenManager.getTime();

		// Debug Buttons
		if (ImGui::Button("Toggle PostFX"))
			ScreenManager.TogglePostFX();

		if (ImGui::Button("Toggle SkyBox"))
			ScreenManager.ToggleSkybox();

		if (ImGui::Button("Toggle Normals Debug"))
			if (debugNormals)
				debugNormals = false;
			else
				debugNormals = true;

		// SHADOW MAPS
		//Shadowbuffer.calculateShadows(sunLight);

		// TERRAIN
		pTerrain->Draw();

		// LIGHTS
		constantShader.use();
		for (int i = 0; i < Lights.size(); i++)
		{
			// spin lights
			Lights[i].position.x = pointLightPositions[i].x + (std::cos(GameTime * 2.0f + i)) * 4.0f;
			Lights[i].position.z = pointLightPositions[i].z + sin(std::sin(GameTime * 2.0f + i)) * 4.0f;
			Lights[i].position.y = pointLightPositions[i].y + std::sin(GameTime * 2.5f + i) * 1.0f;
			if (Lights[i].type == Light::POINT)
			{
				Lights[i].diffuse = glm::mod(Lights[i].position, glm::vec3(1.0));
			}
			//vertex
			glm::mat4 model{ 1.0f };
			model = glm::translate(model, Lights[i].position);
			model = glm::scale(model, glm::vec3(0.3f));
			constantShader.setMat4("model", model);
			constantShader.setMat4("view", MainCamera.viewMat);
			constantShader.setMat4("projection", MainCamera.projectionMat);
			// fragment
			constantShader.setVec3("lightColor", Lights[i].diffuse);
			constantShader.setFloat("lightIntensity", Lights[i].intensity);
			// draw the lamp
			Lamp->Draw(constantShader);
		}

		// RENDER ENTITIES

		litReflectShader.use();
		glm::mat4 model{ 1.0f };
		model = glm::scale(model, glm::vec3(0.3f));
		litReflectShader.setMat4("model", model);
		litReflectShader.setMat4("view", MainCamera.viewMat);
		litReflectShader.setMat4("projection", MainCamera.projectionMat);
		litReflectShader.setFloat("material.ambientBrightness", 0.06f);
		litReflectShader.setFloat("material.roughness", 64.0f);
		litReflectShader.setFloat("material.specularIntensity", 1.0f);
		litReflectShader.setFloat("material.cubemapIntensity", 1.0f);
		updateLightingUniforms(litReflectShader, Lights, MainCamera);
		NanosuitModel->Draw(litReflectShader);

		debugNormalsShader.use();
		debugNormalsShader.setMat4("model", model);
		debugNormalsShader.setMat4("view", MainCamera.viewMat);
		debugNormalsShader.setMat4("projection", MainCamera.projectionMat);
		if(debugNormals)
			NanosuitModel->Draw(debugNormalsShader);

		depthShader.use();
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		depthShader.setMat4("model", model);
		depthShader.setMat4("view", MainCamera.viewMat);
		depthShader.setMat4("projection", MainCamera.projectionMat);
		NanosuitModel->Draw(depthShader);

		refractionShader.use();
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 0.0f));
		refractionShader.setMat4("model", model);
		refractionShader.setMat4("view", MainCamera.viewMat);
		refractionShader.setMat4("projection", MainCamera.projectionMat);
		refractionShader.setVec3("viewPos", MainCamera.get_position());
		NanosuitModel->Draw(refractionShader);

		// CREATING BOXES
		litReflectShader.use(); 
		litReflectShader.setMat4("view", MainCamera.viewMat);
		litReflectShader.setMat4("projection", MainCamera.projectionMat);
		litReflectShader.setFloat("material.ambientBrightness", 0.06f);
		litReflectShader.setFloat("material.roughness", 32.0f);
		litReflectShader.setFloat("material.specularIntensity", 1.0f);
		litReflectShader.setFloat("material.cubemapIntensity", 1.0f);
		updateLightingUniforms(litReflectShader, Lights, MainCamera);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model{ 1.0f };
			model = glm::translate(model, cubePositions[i]);
			float angle = GameTime * ( i + 1 ) * 3.f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			litReflectShader.setMat4("model", model);
			Box->Draw(litReflectShader);
		}

		// CREATING GRASS
		alphaShader.use();
		alphaShader.setMat4("projection", MainCamera.projectionMat);
		alphaShader.setMat4("view", MainCamera.viewMat);

		// Sorting Grass for Transparencey Shading
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < vegetationPositions.size(); i++)
		{
			float distance = glm::length(vegetationPositions[i] - MainCamera.get_position());
			sorted[distance] = vegetationPositions[i];
		}
		// iterating using map
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			glm::mat4 model{ 1.0f };
			model = translate(model, glm::vec3(it->second));
			testShader.setMat4("model", model);
			Plane->Draw(alphaShader);
		}
		ScreenManager.End();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	//// ------------------------------------------------------------------------
	delete Box;
	delete Lamp;
	delete Plane;
	delete pTerrain;
	delete NanosuitModel;

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	ScreenManager.Close();
	return 0;
}

void updateLightingUniforms(Shader& shader, const std::vector<Light>& lights, Camera& camera)
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
		//// lights directional
		shader.setVec3(lightIndex + ".direction", lights[i].direction);
		shader.setVec3(lightIndex + ".position", lights[i].position);
		shader.setVec3(lightIndex + ".diffuse", lights[i].diffuse);
		shader.setFloat(lightIndex + ".intensity", lights[i].intensity);
		//// lights spotlight
		shader.setFloat(lightIndex + ".spotSize", lights[i].spotSize);
		shader.setFloat(lightIndex + ".penumbraSize", lights[i].penumbraSize);
		//// lights point light falloff
		shader.setFloat(lightIndex + ".constant", lights[i].constant);
		shader.setFloat(lightIndex + ".linear", lights[i].linear);
		shader.setFloat(lightIndex + ".quadratic", lights[i].quadratic);
		//// lights view pos
		shader.setVec3("viewPos", camera.get_position());
	}
}

void renderScene(const Shader& shader)
{

}
