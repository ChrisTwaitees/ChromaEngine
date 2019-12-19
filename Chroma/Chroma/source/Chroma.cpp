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
#include <screenManager/ScreenManager.h>
#include <component/IComponent.h>
#include <component/MeshComponent.h>
#include <entity/Entity.h>
#include <scene/Scene.h>
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
	ScreenManager* screenManager{ new ScreenManager };

	// SCENE 
	Scene* scene{ new Scene};

	// GAME
	ChromaGame Game(scene, screenManager);

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
	IEntity* NanosuitEntity = new Entity;
	scene->AddEntity(NanosuitEntity);
	NanosuitEntity->SetName("ChromaSuit");
	NanosuitEntity->SetPosition(glm::vec3(30., 0., 0));

	MeshComponent* NanoSuitModelComponent = new Model("resources/assets/nanosuit/nanosuit.obj");
	PhysicsComponent* NanoSuitRigidComponent = new PhysicsComponent();
	NanoSuitRigidComponent->SetColliderShape(ColliderShape::Convex);
	NanoSuitRigidComponent->SetCollisionState(ColliderState::Dynamic);
	NanoSuitRigidComponent->setMass(1.0f);
	NanoSuitModelComponent->SetShader(&litShader);
	NanosuitEntity->AddComponent(NanoSuitModelComponent);
	NanosuitEntity->AddComponent(NanoSuitRigidComponent);


	std::vector<IEntity*> boxes;
	for (glm::vec3 position : cubePositions)
	{
		IEntity* BoxEntity = new Entity;
		scene->AddEntity(BoxEntity);
		BoxEntity->SetName("Box");
		MeshComponent* BoxMeshComponent = new BoxPrimitive();
		BoxMeshComponent->SetShader(&litShader);
		BoxMeshComponent->AddTexture(diffuseMap);
		BoxMeshComponent->AddTexture(specularMap);
		PhysicsComponent* BoxRigidComponent = new PhysicsComponent();
		BoxRigidComponent->SetColliderShape(ColliderShape::Convex);
		BoxRigidComponent->SetCollisionState(ColliderState::Kinematic);

		BoxEntity->SetPosition(position);
		BoxEntity->AddComponent(BoxMeshComponent);
		BoxEntity->AddComponent(BoxRigidComponent);
		boxes.push_back(BoxEntity);
	}

	std::vector<IEntity*> lamps;
	for (glm::vec3 position : pointLightPositions)
	{
		IEntity* LampEntity = new Entity();
		scene->AddEntity(LampEntity);
		LampEntity->SetName("Lamp");
		MeshComponent* LampMeshComponent = new BoxPrimitive();
		LampMeshComponent->SetShader(&constantShader);
		LampMeshComponent->m_CastShadows = false;
		LampMeshComponent->m_IsLit = false;
		PhysicsComponent* LampRigidComponent = new PhysicsComponent();
		LampRigidComponent->SetColliderShape(ColliderShape::Convex);
		LampRigidComponent->SetCollisionState(ColliderState::Kinematic);

		LampEntity->SetPosition(position);
		LampEntity->AddComponent(LampMeshComponent);
		LampEntity->AddComponent(LampRigidComponent);
		lamps.push_back(LampEntity);
	}

	for (glm::vec3 position : grassPositions)
	{
		IEntity* GrassPlaneEntity = new Entity;
		GrassPlaneEntity->SetName("Grass");
		MeshComponent* GrassPlaneMeshComponent = new PlanePrimitive();
		GrassPlaneMeshComponent->AddTexture(grassMap);
		GrassPlaneMeshComponent->SetShader(&alphaShader);
		GrassPlaneMeshComponent->m_IsLit = false;
		GrassPlaneMeshComponent->m_IsForwardLit = true;
		GrassPlaneMeshComponent->m_IsTransparent = true;
		GrassPlaneEntity->AddComponent(GrassPlaneMeshComponent);
		GrassPlaneEntity->SetPosition(position);
		scene->AddEntity(GrassPlaneEntity);
	}

	// TERRAIN
	IEntity* TerrainEntity = new Entity;
	scene->AddEntity(TerrainEntity);
	MeshComponent* TerrainMeshComponent = new Terrain;
	TerrainMeshComponent->SetShader(&litShader);
	TerrainEntity->AddComponent(TerrainMeshComponent);

	// SPHERE
	IEntity* SphereEntity = new Entity;
	scene->AddEntity(SphereEntity);
	SphereEntity->SetName("Sphere");
	MeshComponent* SphereMeshComponent = new SpherePrimitive;
	PhysicsComponent* SphereRigidComponent = new PhysicsComponent();
	SphereRigidComponent->SetColliderShape(ColliderShape::Convex);
	SphereRigidComponent->SetCollisionState(ColliderState::Kinematic);
	SphereMeshComponent->AddTexture(normalMap);
	SphereMeshComponent->AddTexture(concreteMap);
	SphereMeshComponent->SetShader(&litShader);
	SphereEntity->AddComponent(SphereMeshComponent);
	SphereEntity->AddComponent(SphereRigidComponent);


	// POPULATING SCENE
	scene->SetLights(Lights);


	// RENDER LOOP
	// -----------
	while (!screenManager->shouldClose())
	{
		// SCREENMANAGER START
		screenManager->StartLoop();
		float GameTime = screenManager->getTime();
		float DeltaTime = screenManager->GetDeltaTime();

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
				Game.getRenderer()->GetDebugBuffer()->DrawLine(glm::vec3(0), newLightPos, glm::vec3(1, 1, 0));
			}
			// fragments
			for (IComponent* component : lamps[i]->getRenderableComponents())
			{
				glm::vec3 color(Lights[i]->getDiffuse()* Lights[i]->getIntensity());
				((MeshComponent*)component)->GetShader()->setVec3("color", glm::vec4(color, 1.0));

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
		screenManager->EndLoop();
	}


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	screenManager->Close();
	return 0;
}