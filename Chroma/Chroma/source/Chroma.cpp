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
#include "renderer/Renderer.h"


int main()
{
	// SCREEN MANAGER
	ChromaScreenManager ScreenManager;
	Camera* MainCamera = ScreenManager.getActiveCamera();

	// SCENE 
	ChromaScene* Scene = new ChromaScene;

	// LIGHTS
	std::vector<std::shared_ptr<Light>> Lights;


	// SCENE CONTENTS
	// ---------------
	// point light positions
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  2.2f,  2.0f),
		glm::vec3(2.3f, -1.3f, -4.0f),
		glm::vec3(-4.0f,  4.0f, -12.0f),
		glm::vec3(0.0f,  2.0f, -3.0f)
	};
	// dancing point lights
	for (glm::vec3 pos : pointLightPositions)
	{
		std::shared_ptr<Light> pointLight = std::make_shared < Light >(pos, Light::POINT);
		pointLight->intensity *= 5.0f;
		pointLight->quadratic *= 4.0f;
		pointLight->linear *= 2.0f;
		Lights.push_back(pointLight);
	}
	// SUNLIGHT
	std::shared_ptr<Light> Sun = std::make_shared<Light>(Light::SUNLIGHT, glm::vec3(0.2, -0.8, 0.3), 1.0f);

	Lights.push_back(Sun);

	// CUBES
	// Dancing cubes
	glm::vec3 cubePositions[] = {
		  glm::vec3(0.0f,  3.0f,  0.0f),
		  glm::vec3(2.0f,  8.0f, -15.0f),
		  glm::vec3(-1.5f, 1.2f, -2.5f),
		  glm::vec3(-3.8f, 1.0f, -12.3f),
		  glm::vec3(2.4f, 3.4f, -3.5f),
		  glm::vec3(-1.7f,  6.0f, -7.5f),
		  glm::vec3(1.3f, 1.0f, -2.5f),
		  glm::vec3(1.5f,  5.0f, -2.5f),
		  glm::vec3(1.5f,  3.2f, -1.5f),
		  glm::vec3(-1.3f,  4.0f, -1.5f)
	};

	// Grass Positions
	std::vector<glm::vec3> vegetationPositions;
	vegetationPositions.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	vegetationPositions.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	vegetationPositions.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	vegetationPositions.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetationPositions.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	// SHADERS
	Shader litNormalsShader("resources/shaders/fragLitShadowsNormals.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
	Shader litShader("resources/shaders/fragLitShadows.glsl", "resources/shaders/vertexLitShadows.glsl");
	Shader refractionShader("resources/shaders/fragRefraction.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader constantShader("resources/shaders/fragConstant.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader alphaShader("resources/shaders/fragAlpha.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader debugNormalsShader("resources/shaders/fragDebugNormals.glsl", "resources/shaders/vertexDebugNormals.glsl", "resources/shaders/geometryDebugNormals.glsl");


	// TEXTURES
	Texture diffuseMap("resources/textures/wooden_panel.png");
	Texture specularMap("resources/textures/wooden_panel_specular.png");
	Texture grassMap("resources/textures/grass.png");
	Texture terrainTex("resources/textures/terrain1.jpeg");

	// ENTITIES
	std::vector<ChromaEntity*> Entities;

	ChromaEntity * NanosuitEntity = new ChromaEntity;
	ChromaComponent * NanoSuitModelComponent = new Model("resources/assets/nanosuit/nanosuit.obj");
	NanoSuitModelComponent->bindShader(&litNormalsShader);
	NanosuitEntity->addComponent(NanoSuitModelComponent);
	Entities.push_back(NanosuitEntity);

	std::vector<ChromaEntity*> boxes;
	for (glm::vec3 position : cubePositions)
	{
		ChromaEntity* BoxEntity = new ChromaEntity;
		ChromaComponent* BoxMeshComponent = new BoxPrimitive;
		BoxMeshComponent->bindShader(&litShader);
		BoxMeshComponent->bindTexture(diffuseMap);
		BoxMeshComponent->bindTexture(specularMap);
		BoxEntity->addComponent(BoxMeshComponent);
		BoxEntity->setPosition(position);
		boxes.push_back(BoxEntity);
		Entities.push_back(BoxEntity);
	}

	std::vector<ChromaEntity*> lamps;
	for (glm::vec3 position : pointLightPositions)
	{
		ChromaEntity* LampEntity = new ChromaEntity;
		ChromaComponent* LampMeshComponent = new BoxPrimitive;
		LampMeshComponent->bindShader(&constantShader);
		LampEntity->addComponent(LampMeshComponent);
		LampEntity->setPosition(position);
		lamps.push_back(LampEntity);
		Entities.push_back(LampEntity);
	}

	//ChromaEntity* GrassPlaneEntity = new ChromaEntity;
	//ChromaComponent* GrassPlaneMeshComponent = new PlanePrimitive;
	//GrassPlaneMeshComponent->bindTexture(grassMap);
	//GrassPlaneMeshComponent->bindShader(&alphaShader);
	//GrassPlaneEntity->addComponent(GrassPlaneMeshComponent);

	ChromaEntity* TerrainEntity = new ChromaEntity;
	ChromaComponent* TerrainMeshComponent = new Terrain;
	TerrainMeshComponent->bindShader(&litNormalsShader);
	TerrainEntity->addComponent(TerrainMeshComponent);
	Entities.push_back(TerrainEntity);

	// POPULATING SCENE
	Scene->setEntities(Entities);
	Scene->setLights(Lights);
	Scene->setRenderCamera(MainCamera);

	// RENDERER
	Renderer Renderer(Scene, &ScreenManager);


	// RENDER LOOP
	// -----------
	while (!ScreenManager.shouldClose())
	{
		// SCREENMANAGER START
		ScreenManager.Start();
		float GameTime = ScreenManager.getTime();
		float DeltaTime = ScreenManager.getDeltaTime();

		if (ImGui::Button("Toggle SkyBox"))
			ScreenManager.ToggleSkybox();


		ImGui::SliderFloat("Exposure", &ScreenManager.exposure, 0.0f, 2.0f);

		if (ImGui::Button("Use Bloom"))
			ScreenManager.ToggleBloom();

		// SHADOW MAPS
		Sun->position = Sun->direction * -20.0f;

		// LIGHTS
		constantShader.use();
		for (int i = 0; i < Lights.size() - 1; i++)
		{
			if (Lights[i]->type == Light::SUNLIGHT)
			{
				//positions
				lamps[i]->scale(glm::vec3(0.3f));
			}
			// spin lights
			if (Lights[i]->type == Light::POINT)
			{
				Lights[i]->position.x = pointLightPositions[i].x + (std::cos(GameTime * 2.0f + i)) * 4.0f;
				Lights[i]->position.z = pointLightPositions[i].z + sin(std::sin(GameTime * 2.0f + i)) * 4.0f;
				Lights[i]->position.y = pointLightPositions[i].y + std::sin(GameTime * 2.5f + i) * 4.0f;
				Lights[i]->diffuse = glm::mod(Lights[i]->position, glm::vec3(1.0));
				//positions
				lamps[i]->setPosition(Lights[i]->position);
				lamps[i]->scale(glm::vec3(0.3f));
			}
			// fragments
			for (ChromaComponent* component : lamps[i]->RenderableComponents)
			{
				component->getShader()->setVec3("lightColor", Lights[i]->diffuse);
				component->getShader()->setFloat("lightIntensity", Lights[i]->intensity);
			}
		}

		// RENDER ENTITIES
		NanosuitEntity->setScale(glm::vec3(0.5f));
		NanosuitEntity->rotate(glm::mod(GameTime, 360.0f) * 10.f, glm::vec3(0.0f, 1.0f, 0.0f));

		//Sunlight Rotation
		Sun->direction = glm::normalize((glm::vec3(std::sin(GameTime * 1.0f), -glm::abs(std::sin(GameTime * 1.0f)), -std::cos(GameTime * 1.0f))));


		// SPINNING BOXES
		for (unsigned int i = 0; i < boxes.size(); i++)
		{
			float angle = DeltaTime * (i + 1) * 6.0f;
			boxes[i]->rotate(angle, glm::vec3(1.0f, 0.3f, 0.5f));
		}

		// CREATING GRASS
		// Sorting Grass for Transparencey Shading
		//std::map<float, glm::vec3> sorted;
		//for (unsigned int i = 0; i < vegetationPositions.size(); i++)
		//{
		//	float distance = glm::length(vegetationPositions[i] - MainCamera->get_position());
		//	sorted[distance] = vegetationPositions[i];
		//}
		//// iterating using map
		//for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		//{
		//	GrassPlaneEntity->setPosition(glm::vec3(it->second));
		//}

		// RENDER
		Renderer.RenderScene();

		// END RENDER LOOP
		ScreenManager.End();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	//// ------------------------------------------------------------------------
//	delete GrassPlaneEntity;
	delete TerrainEntity;
	delete NanosuitEntity;
	delete MainCamera;
	delete Scene;


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	ScreenManager.Close();
	return 0;
}