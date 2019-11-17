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

// chroma
#include <screenManager/ChromaScreenManager.h>
#include <component/IChromaComponent.h>
#include <component/ChromaMeshComponent.h>
#include <entity/ChromaEntity.h>
#include <scene/ChromaScene.h>
#include <model/Model.h>
#include <model/BoxPrimitive.h>
#include <model/PlanePrimitive.h>
#include <model/SpherePrimitive.h>
#include <model/LinePrimitive.h>
#include <shader/Shader.h>
#include <texture/Texture.h>
#include <camera/Camera.h>
#include <light/Light.h>
#include <terrain/Terrain.h>
#include <renderer/Renderer.h>
#include <game/ChromaGame.h>


int main()
{
	

	// SCREEN MANAGER
	ChromaScreenManager* ScreenManager{ new ChromaScreenManager };

	// SCENE 
	ChromaScene* Scene{ new ChromaScene };

	// GAME
	ChromaGame Game(Scene, ScreenManager);

	// LIGHTS
	std::vector<std::shared_ptr<Light>> Lights;

	// SCENE CONTENTS
	// ---------------
	// point light positions
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  2.2f,  2.0f),
		glm::vec3(2.3f, -1.3f, -4.0f),
		glm::vec3(-4.0f,  4.0f, -12.0f),
		glm::vec3(1.0f,  1.0f, -3.0f),
		glm::vec3(2.0f,  -3.0f, -1.0f),
		glm::vec3(4.0f,  -2.0f, 3.0f),
		glm::vec3(-1.0f,  2.0f, -3.0f)
	};
	// dancing point lights
	for (glm::vec3 pos : pointLightPositions)
	{
		std::shared_ptr<Light> pointLight = std::make_shared < Light >(pos, Light::POINT);
		pointLight->setIntensity(3.0f);
		pointLight->quadratic *= 4.0f;
		pointLight->linear *= 2.0f;
		Lights.push_back(pointLight);
	}
	// SUNLIGHT
	std::shared_ptr<Light> Sun = std::make_shared<Light>(Light::SUNLIGHT, glm::vec3(0.2, -0.8, 0.3), 0.8f);
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
	std::vector<glm::vec3> grassPositions;
	grassPositions.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	grassPositions.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	grassPositions.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	grassPositions.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	grassPositions.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

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
	specularMap.type = Texture::SPECULAR;
	Texture concreteMap("resources/textures/brickwall.jpg");
	Texture normalMap("resources/textures/brickwall_normal.jpg");
	normalMap.type = Texture::NORMAL;
	Texture grassMap("resources/textures/grass.png");
	Texture terrainTex("resources/textures/terrain1.jpeg");

	// ENTITIES
	std::vector<IChromaEntity*> Entities;

	IChromaEntity* NanosuitEntity = new ChromaEntity;
	ChromaMeshComponent* NanoSuitModelComponent = new Model("resources/assets/nanosuit/nanosuit.obj");
	ChromaPhysicsComponent* NanoSuitRigidComponent = new ChromaPhysicsComponent();
	NanoSuitModelComponent->bindShader(&litNormalsShader);
	NanosuitEntity->addComponent(NanoSuitModelComponent);
	NanosuitEntity->addComponent(NanoSuitRigidComponent);
	Entities.push_back(NanosuitEntity);


	std::vector<IChromaEntity*> boxes;
	for (glm::vec3 position : cubePositions)
	{
		IChromaEntity* BoxEntity = new ChromaEntity;
		ChromaMeshComponent* BoxMeshComponent = new BoxPrimitive;
		BoxMeshComponent->bindShader(&litShader);
		BoxMeshComponent->bindTexture(diffuseMap);
		BoxMeshComponent->bindTexture(specularMap);
		BoxEntity->addComponent(BoxMeshComponent);
		BoxEntity->setPosition(position);
		boxes.push_back(BoxEntity);
		Entities.push_back(BoxEntity);
	}

	std::vector<IChromaEntity*> lamps;
	for (glm::vec3 position : pointLightPositions)
	{
		IChromaEntity* LampEntity = new ChromaEntity;
		ChromaMeshComponent* LampMeshComponent = new BoxPrimitive;
		LampMeshComponent->bindShader(&constantShader);
		LampMeshComponent->castShadows = false;
		LampMeshComponent->isLit = false;
		LampEntity->addComponent(LampMeshComponent);
		LampEntity->setPosition(position);
		lamps.push_back(LampEntity);
		Entities.push_back(LampEntity);
	}

	//for (glm::vec3 position : grassPositions)
	//{
	//	ChromaEntity* GrassPlaneEntity = new ChromaEntity;
	//	IChromaComponent* GrassPlaneMeshComponent = new PlanePrimitive;
	//	GrassPlaneMeshComponent->bindTexture(grassMap);
	//	GrassPlaneMeshComponent->bindShader(&alphaShader);
	//	GrassPlaneEntity->addComponent(GrassPlaneMeshComponent);
	//	Entities.push_back(GrassPlaneEntity);
	//}

	// TERRAIN
	IChromaEntity* TerrainEntity = new ChromaEntity;
	ChromaMeshComponent* TerrainMeshComponent = new Terrain;
	TerrainMeshComponent->bindShader(&litNormalsShader);
	TerrainEntity->addComponent(TerrainMeshComponent);
	Entities.push_back(TerrainEntity);

	// SPHERE
	IChromaEntity* SphereEntity = new ChromaEntity;
	ChromaMeshComponent* SphereMeshComponent = new SpherePrimitive;
	SphereMeshComponent->bindTexture(normalMap);
	SphereMeshComponent->bindTexture(concreteMap);
	SphereMeshComponent->bindShader(&litNormalsShader);
	SphereEntity->addComponent(SphereMeshComponent);
	Entities.push_back(SphereEntity);

	// LINE
	IChromaEntity* LineEntity = new ChromaEntity;
	ChromaMeshComponent* LineMeshComponent = new LinePrimitive{glm::vec3(0), glm::vec3(1)};
	LineMeshComponent->isLit = false;
	LineEntity->addComponent(LineMeshComponent);
	Entities.push_back(LineEntity);

	// POPULATING SCENE
	Scene->setEntities(Entities);
	Scene->setLights(Lights);


	// RENDER LOOP
	// -----------
	while (!ScreenManager->shouldClose())
	{
		// SCREENMANAGER START
		ScreenManager->Start();
		float GameTime = ScreenManager->getTime();
		float DeltaTime = ScreenManager->getDeltaTime();

		// Updating sun
		Sun->setPosition(Sun->getDirection() * -20.0f);
		//Sunlight Rotation
		Sun->setDirection(glm::normalize((glm::vec3(std::sin(GameTime * 1.0f), -glm::abs(std::sin(GameTime * 1.0f)), -std::cos(GameTime * 1.0f)))));

		LineMeshComponent->setFloat("time", GameTime);

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
				glm::vec3 newLightPos;

				newLightPos.x = pointLightPositions[i].x + (std::cos(GameTime * 2.0f + i)) * 4.0f;
				newLightPos.z = pointLightPositions[i].z + sin(std::sin(GameTime * 2.0f + i)) * 4.0f;
				newLightPos.y = pointLightPositions[i].y + std::sin(GameTime * 2.5f + i) * 4.0f;
				Lights[i]->setPosition(newLightPos);

				Lights[i]->setDiffuse(glm::mod(newLightPos, glm::vec3(1.0)));
				//positions
				lamps[i]->setPosition(newLightPos);
				lamps[i]->scale(glm::vec3(0.3f));
				SphereEntity->setPosition(newLightPos);
				LineMeshComponent->setEndPos(newLightPos);
			}
			// fragments
			for (IChromaComponent* component : lamps[i]->getRenderableComponents())
			{
				((ChromaMeshComponent*)component)->getShader()->setVec3("lightColor", Lights[i]->getDiffuse());
				((ChromaMeshComponent*)component)->getShader()->setFloat("lightIntensity", Lights[i]->getIntensity());
			}
				
		}

		// NANO SUIT
		NanosuitEntity->setScale(glm::vec3(0.5f));
		NanosuitEntity->rotate(glm::mod(GameTime, 360.0f) * 10.f, glm::vec3(0.0f, 1.0f, 0.0f));		

		// SPINNING BOXES
		for (unsigned int i = 0; i < boxes.size(); i++)
		{
			float angle = DeltaTime * (i + 1) * 6.0f;
			boxes[i]->rotate(angle, glm::vec3(1.0f, 0.3f, 0.5f));

		}


		// GAME TICK
		Game.Tick();

		// END SCREEN
		ScreenManager->End();
	}


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	ScreenManager->Close();
	return 0;
}