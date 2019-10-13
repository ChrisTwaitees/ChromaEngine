// c++ standard libs
#include <math.h>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
//// glad and glfw
#include <glad/glad.h>
// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// chroma
#include "screenManager/ChromaScreenManager.h"
#include "component/ChromaComponent.h"
#include "entity/ChromaEntity.h"
#include "scene/ChromaScene.h"
#include "buffers/Framebuffer.h"
#include "buffers/ShadowBuffer.h"
#include "models/Model.h"
#include "models/BoxPrimitive.h"
#include "models/PlanePrimitive.h"
#include "models/SkyBox.h"
#include "shaders/Shader.h"
#include "texture/Texture.h"
#include "cameras/Camera.h"
#include "lights/Light.h"
#include "terrain/Terrain.h"


int main()
{
	// SCREEN MANAGER
	ChromaScreenManager ScreenManager;
	Camera* MainCamera = ScreenManager.getActiveCamera();

	// SCENE 
	ChromaScene* Scene = new ChromaScene;

	// ENTITIES
	std::vector<ChromaEntity*> Entities;

	// LIGHTS
	std::vector<Light*> Lights;

	// RENDERER
	Renderer Renderer(Scene);

	// Shadowbuffer
	//ShadowBuffer Shadowbuffer;
	
	// SCENE CONTENTS
	// ---------------
	// point light positions
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	// dancing point lights
	for (glm::vec3 pos : pointLightPositions)
	{
		Light pointLight(pos, Light::POINT);
		Lights.push_back(&pointLight);
	}
	// default spot and dir light
	Light sunLight(Light::DIRECTIONAL, glm::vec3(0.2, -0.8, 0.0), 1.75f);
	Light spotLight(Light::SPOT, glm::vec3(0.0f), 0.0f);
	Lights.push_back(&sunLight);

	// CUBES
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
	Shader nanoSuitShader("resources/shaders/fragLitReflect.glsl", "resources/shaders/vertexShaderLighting.glsl");
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

	//// ENTITIES POPULATION
	//ChromaEntity* NanosuitEntity = new ChromaEntity;
	//ChromaComponent* NanoSuitModelComponent = new Model("resources/assets/nanosuit/nanosuit.obj");
	//NanosuitEntity->addComponent(NanoSuitModelComponent);
	//Entities.push_back(NanosuitEntity);

	ChromaEntity* BoxEntity = new ChromaEntity;
	ChromaComponent* BoxMeshComponent = new BoxPrimitive;
	BoxMeshComponent->bindShader(&nanoSuitShader);
	BoxMeshComponent->bindTexture(diffuseMap);
	BoxMeshComponent->bindTexture(specularMap);
	BoxEntity->addComponent(BoxMeshComponent);
	Entities.push_back(BoxEntity);

	ChromaEntity* LampEntity = new ChromaEntity;
	ChromaComponent* LampMeshComponent = new BoxPrimitive;
	LampMeshComponent->bindShader(&constantShader);
	LampEntity->addComponent(LampMeshComponent);
	Entities.push_back(LampEntity);
	
	ChromaEntity* GrassPlaneEntity = new ChromaEntity;
	ChromaComponent* GrassPlaneMeshComponent =  new PlanePrimitive;
	GrassPlaneMeshComponent->bindTexture(grassMap);
	GrassPlaneMeshComponent->bindShader(&alphaShader);
	GrassPlaneEntity->addComponent(GrassPlaneMeshComponent);
	Entities.push_back(GrassPlaneEntity);

	ChromaEntity* TerrainEntity = new ChromaEntity;
	ChromaComponent* TerrainMeshComponent = new Terrain;
	TerrainMeshComponent->bindTexture(terrainTex);
	TerrainMeshComponent->bindShader(&litReflectShader);
	TerrainEntity->addComponent(TerrainMeshComponent);
	Entities.push_back(TerrainEntity);

	// POPULATING SCENE
	Scene->setEntities(Entities);
	Scene->setLights(Lights);
	Scene->setRenderCamera(MainCamera);

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

		// LIGHTS
		constantShader.use();
		for (int i = 0; i < Lights.size(); i++)
		{
			// spin lights
			Lights[i]->position.x = pointLightPositions[i].x + (std::cos(GameTime * 2.0f + i)) * 4.0f;
			Lights[i]->position.z = pointLightPositions[i].z + sin(std::sin(GameTime * 2.0f + i)) * 4.0f;
			Lights[i]->position.y = pointLightPositions[i].y + std::sin(GameTime * 2.5f + i) * 1.0f;
			if (Lights[i]->type == Light::POINT)
				Lights[i]->diffuse = glm::mod(Lights[i]->position, glm::vec3(1.0));
			//positions
			LampEntity->setPosition(Lights[i]->position);
			LampEntity->scale(glm::vec3(0.3f));
			// fragments
			LampMeshComponent->getShader()->setVec3("lightColor", Lights[i]->diffuse);
			LampMeshComponent->getShader()->setFloat("lightIntensity", Lights[i]->intensity);
		}

		// RENDER ENTITIES
		//NanosuitEntity->scale(glm::vec3(0.3f));

		//if(debugNormals)
		//	NanosuitEntity->Draw(debugNormalsShader);

		//Nanosuit->translate(glm::vec3(10.0f, 0.0f, 0.0f));
		//Nanosuit->Draw(depthShader);
		//Nanosuit->Draw(refractionShader);

		// CREATING BOXES
		for (unsigned int i = 0; i < 10; i++)
		{
			BoxEntity->setPosition(cubePositions[i]);
			float angle = GameTime * ( i + 1 ) * 3.f;
			BoxEntity->rotate(angle, glm::vec3(1.0f, 0.3f, 0.5f));
		}

		// CREATING GRASS
		// Sorting Grass for Transparencey Shading
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < vegetationPositions.size(); i++)
		{
			float distance = glm::length(vegetationPositions[i] - MainCamera->get_position());
			sorted[distance] = vegetationPositions[i];
		}
		// iterating using map
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			GrassPlaneEntity->setPosition(glm::vec3(it->second));
		}

		// RENDER
		Renderer.Render();

		// END RENDER LOOP
		ScreenManager.End();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	//// ------------------------------------------------------------------------
	delete BoxEntity;
	delete LampEntity;
	delete GrassPlaneEntity;
	delete TerrainEntity;
//	delete NanosuitEntity;
	delete MainCamera;
	delete Scene;


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	ScreenManager.Close();
	return 0;
}
