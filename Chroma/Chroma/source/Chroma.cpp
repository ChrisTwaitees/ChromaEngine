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
#include <game/ChromaGame.h>


int Main()
{

	// INIT CHROMA
	// ------------------------------------------------------------------------------------------

	// SCREEN MANAGER
	ChromaScreenManager* ScreenManager{ new ChromaScreenManager };

	// SCENE 
	ChromaScene* Scene{ new ChromaScene };

	// GAME
	ChromaGame Game(Scene, ScreenManager);

	// SCENE CONTENTS
	// ------------------------------------------------------------------------------------------

	// LIGHTS
	std::vector<Light*> Lights;

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
		Light* pointLight = new Light(pos, Light::POINT);
		pointLight->setIntensity(3.0f);
		pointLight->quadratic *= 4.0f;
		pointLight->linear *= 2.0f;
		Lights.push_back(pointLight);
	}
	// SUNLIGHT
	Light* Sun = new Light(Light::SUNLIGHT, glm::vec3(0.2, -0.8, 0.3), 0.8f);
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
	Shader litShader("resources/shaders/fragLitShadowsNormals.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
	Shader refractionShader("resources/shaders/fragRefraction.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader constantShader("resources/shaders/fragBasic.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader alphaShader("resources/shaders/fragAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
	Shader debugNormalsShader("resources/shaders/fragDebugNormals.glsl", "resources/shaders/vertexDebugNormals.glsl", "resources/shaders/geometryDebugNormals.glsl");


	// TEXTURES
	Texture diffuseMap("resources/textures/wooden_panel.png");
	Texture specularMap("resources/textures/wooden_panel_specular.png");
	specularMap.type = Texture::METALNESS;
	Texture concreteMap("resources/textures/brickwall.jpg");
	Texture normalMap("resources/textures/brickwall_normal.jpg");
	normalMap.type = Texture::NORMAL;
	Texture grassMap("resources/textures/grass.png");
	grassMap.type = Texture::ALBEDO;
	Texture terrainTex("resources/textures/terrain1.jpeg");

	// ENTITIES
	IChromaEntity* NanosuitEntity = new ChromaEntity;
	Scene->addEntity(NanosuitEntity);
	NanosuitEntity->SetName("ChromaSuit");
	NanosuitEntity->SetPosition(glm::vec3(30., 0., 0));

	ChromaMeshComponent* NanoSuitModelComponent = new Model("resources/assets/nanosuit/nanosuit.obj");
	ChromaPhysicsComponent* NanoSuitRigidComponent = new ChromaPhysicsComponent();
	NanoSuitRigidComponent->SetColliderShape(ColliderShape::Convex);
	NanoSuitRigidComponent->SetCollisionState(ColliderState::Dynamic);
	NanoSuitRigidComponent->setMass(1.0f);
	NanoSuitModelComponent->SetShader(&litShader);
	NanosuitEntity->addComponent(NanoSuitModelComponent);
	NanosuitEntity->addComponent(NanoSuitRigidComponent);


	std::vector<IChromaEntity*> boxes;
	for (glm::vec3 position : cubePositions)
	{
		IChromaEntity* BoxEntity = new ChromaEntity;
		Scene->addEntity(BoxEntity);
		BoxEntity->SetName("Box");
		ChromaMeshComponent* BoxMeshComponent = new BoxPrimitive();
		BoxMeshComponent->SetShader(&litShader);
		BoxMeshComponent->AddTexture(diffuseMap);
		BoxMeshComponent->AddTexture(specularMap);
		ChromaPhysicsComponent* BoxRigidComponent = new ChromaPhysicsComponent();
		BoxRigidComponent->SetColliderShape(ColliderShape::Convex);
		BoxRigidComponent->SetCollisionState(ColliderState::Kinematic);

		BoxEntity->SetPosition(position);
		BoxEntity->addComponent(BoxMeshComponent);
		BoxEntity->addComponent(BoxRigidComponent);
		boxes.push_back(BoxEntity);
	}

	std::vector<IChromaEntity*> lamps;
	for (glm::vec3 position : pointLightPositions)
	{
		IChromaEntity* LampEntity = new ChromaEntity();
		Scene->addEntity(LampEntity);
		LampEntity->SetName("Lamp");
		ChromaMeshComponent* LampMeshComponent = new BoxPrimitive();
		LampMeshComponent->SetShader(&constantShader);
		LampMeshComponent->m_CastShadows = false;
		LampMeshComponent->m_IsLit = false;
		ChromaPhysicsComponent* LampRigidComponent = new ChromaPhysicsComponent();
		LampRigidComponent->SetColliderShape(ColliderShape::Convex);
		LampRigidComponent->SetCollisionState(ColliderState::Kinematic);

		LampEntity->SetPosition(position);
		LampEntity->addComponent(LampMeshComponent);
		LampEntity->addComponent(LampRigidComponent);
		lamps.push_back(LampEntity);
	}

	for (glm::vec3 position : grassPositions)
	{
		IChromaEntity* GrassPlaneEntity = new ChromaEntity;
		GrassPlaneEntity->SetName("Grass");
		ChromaMeshComponent* GrassPlaneMeshComponent = new PlanePrimitive();
		GrassPlaneMeshComponent->AddTexture(grassMap);
		GrassPlaneMeshComponent->SetShader(&alphaShader);
		GrassPlaneMeshComponent->m_IsLit = false;
		GrassPlaneMeshComponent->m_IsForwardLit = true;
		GrassPlaneMeshComponent->m_IsTransparent = true;
		GrassPlaneEntity->addComponent(GrassPlaneMeshComponent);
		GrassPlaneEntity->SetPosition(position);
		Scene->addEntity(GrassPlaneEntity);
	}

	// TERRAIN
	IChromaEntity* TerrainEntity = new ChromaEntity;
	Scene->addEntity(TerrainEntity);
	ChromaMeshComponent* TerrainMeshComponent = new Terrain;
	TerrainMeshComponent->SetShader(&litShader);
	TerrainEntity->addComponent(TerrainMeshComponent);

	// SPHERE
	IChromaEntity* SphereEntity = new ChromaEntity;
	Scene->addEntity(SphereEntity);
	SphereEntity->SetName("Sphere");
	ChromaMeshComponent* SphereMeshComponent = new SpherePrimitive;
	ChromaPhysicsComponent* SphereRigidComponent = new ChromaPhysicsComponent();
	SphereRigidComponent->SetColliderShape(ColliderShape::Convex);
	SphereRigidComponent->SetCollisionState(ColliderState::Kinematic);
	SphereMeshComponent->AddTexture(normalMap);
	SphereMeshComponent->AddTexture(concreteMap);
	SphereMeshComponent->SetShader(&litShader);
	SphereEntity->addComponent(SphereMeshComponent);
	SphereEntity->addComponent(SphereRigidComponent);


	// POPULATING SCENE
	Scene->setLights(Lights);


	// RENDER LOOP
	// -----------
	while (!ScreenManager->shouldClose())
	{
		// SCREENMANAGER START
		ScreenManager->StartLoop();
		float GameTime = ScreenManager->getTime();
		float DeltaTime = ScreenManager->getDeltaTime();

		// Updating sun
		Sun->SetPosition(Sun->GetDirection() * -20.0f);
		//Sunlight Rotation
		Sun->setDirection(glm::normalize((glm::vec3(std::sin(GameTime * 1.0f), -glm::abs(std::sin(GameTime * 1.0f)), -std::cos(GameTime * 1.0f)))));

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
				Lights[i]->SetPosition(newLightPos);

				Lights[i]->setDiffuse(glm::mod(newLightPos, glm::vec3(1.0)));
				//positions
				lamps[i]->SetPosition(newLightPos);
				lamps[i]->scale(glm::vec3(0.3f));
				SphereEntity->SetPosition(newLightPos);
				Game.getRenderer()->getDebugBuffer()->drawLine(glm::vec3(0), newLightPos, glm::vec3(1, 1, 0));
			}
			// fragments
			for (IChromaComponent* component : lamps[i]->getRenderableComponents())
			{
				glm::vec3 color(Lights[i]->getDiffuse()* Lights[i]->getIntensity());
				((ChromaMeshComponent*)component)->GetShader()->setVec3("color", glm::vec4(color, 1.0));

			}

		}

		// NANO SUIT

		//NanosuitEntity->rotate(glm::mod(GameTime, 360.0f) * 10.f, glm::vec3(0.0f, 1.0f, 0.0f));		

		// SPINNING BOXES
		for (unsigned int i = 0; i < boxes.size(); i++)
		{
			float angle = DeltaTime * (i + 1) * 6.0f;
			boxes[i]->rotate(angle, glm::vec3(1.0f, 0.3f, 0.5f));

		}


		// GAME TICK
		Game.Tick();

		// END SCREEN
		ScreenManager->EndLoop();
	}


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	ScreenManager->Close();
	return 0;
}