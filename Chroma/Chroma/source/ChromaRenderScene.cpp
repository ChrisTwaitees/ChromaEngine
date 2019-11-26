// c++ standard libs
#include <iostream>
#include <vector>
#include <map>
//// glad and glfw
#include <glad/glad.h>
// glm
#include <glm/glm.hpp>

// chroma
#include <screenManager/ChromaScreenManager.h>
#include <component/IChromaComponent.h>
#include <component/ChromaMeshComponent.h>
#include <entity/ChromaEntity.h>
#include <scene/ChromaScene.h>
#include <model/Model.h>
#include <model/BoxPrimitive.h>
#include <model/SpherePrimitive.h>
#include <shader/Shader.h>
#include <texture/Texture.h>
#include <light/Light.h>
#include <terrain/Terrain.h>
#include <game/ChromaGame.h>


int main()
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
	const float SUNLIGHT_SPIN_SPEED = .8f;
	const float SUNLIGHT_DISTANCE = 15.0f;

	std::vector<Light*> Lights;

	// point light positions
	glm::vec3 pointLightPositions[] = {
		glm::vec3(2.5f,  1.2f,  2.0f),
	};
	// dancing point lights
	for (glm::vec3 pos : pointLightPositions)
	{
		Light* pointLight = new Light(pos, Light::POINT);
		pointLight->setIntensity(1.0f);
		pointLight->quadratic *= 4.0f;
		pointLight->linear *= 2.0f;
		Lights.push_back(pointLight);
	}
	// SUNLIGHT
	Light* Sun = new Light(Light::SUNLIGHT, glm::vec3(0.2, -0.8, 0.3), 2.0f);
	Sun->setDiffuse(glm::vec3(1.0));
	Sun->setIntensity(3.0);
	Lights.push_back(Sun);


	// SHADERS
	Shader UnlitShader("resources/shaders/fragBasic.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
	UnlitShader.use();
	UnlitShader.setVec3("color", glm::vec3(1, 1, 0));
	Shader SemiTransparentShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
	Shader PBRShader("resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");

	// TEXTURES
	Texture blackAlbedo("resources/textures/colors/black.jpg");
	Texture greyAlbedo("resources/textures/colors/grey.jpg");
	Texture whiteAlbedo("resources/textures/colors/white.jpg");
	Texture alphaTestAlbedo("resources/textures/test/alphatest.png");
	Texture sandyNormal("resources/textures/test/sandy_normal.jpg");
	sandyNormal.type = Texture::NORMAL;

	// AgedWoodplanks
	Texture agedPlanksAlbedo("resources/textures/agedplanks_pbr/albedo.jpg");
	agedPlanksAlbedo.type = Texture::ALBEDO;
	Texture agedPlanksNormal("resources/textures/agedplanks_pbr/normal.jpg");
	agedPlanksNormal.type = Texture::NORMAL;
	Texture agedPlanksMetRoughAO("resources/textures/agedplanks_pbr/MetRoughAO.jpg");
	agedPlanksMetRoughAO.type = Texture::METROUGHAO;

	// Rusted Metal
	Texture rustedIronAlbedo("resources/textures/rustediron_pbr/albedo.jpg");
	rustedIronAlbedo.type = Texture::ALBEDO;
	Texture rustedIronNormal("resources/textures/rustediron_pbr/normal.jpg");
	rustedIronNormal.type = Texture::NORMAL;
	Texture rustedIronMetRoughAO("resources/textures/rustediron_pbr/MetRoughAO.jpg");
	rustedIronMetRoughAO.type = Texture::METROUGHAO;

	// Lookdev Sphere
	Texture lookdevAlbedo("resources/textures/lookdev_pbr/albedo.jpg");
	lookdevAlbedo.type = Texture::ALBEDO;
	Texture lookdevNormal("resources/textures/lookdev_pbr/normal.jpg");
	lookdevNormal.type = Texture::NORMAL;
	Texture lookdevMetRoughAO("resources/textures/lookdev_pbr/MetRoughAO.jpg");
	lookdevMetRoughAO.type = Texture::METROUGHAO;

	// TERRAIN
	IChromaEntity* TerrainEntity = new ChromaEntity;
	Scene->addEntity(TerrainEntity);
	ChromaMeshComponent* TerrainMeshComponent = new Terrain;
	TerrainMeshComponent->bindTexture(greyAlbedo);
	TerrainMeshComponent->bindTexture(sandyNormal);
	TerrainMeshComponent->bindShader(&PBRShader);
	TerrainEntity->addComponent(TerrainMeshComponent);

	// SPHERES
	// Sphere Positions
	glm::vec3 spherePositions[] = {
		glm::vec3(0.f,  1.0f,  0.0f),
		glm::vec3(2.5f,  1.0f,  0.0f),
		glm::vec3(5.0f,  1.0f,  0.0f)
	};

	
	for (glm::vec3 position : spherePositions)
	{
		IChromaEntity* SphereEntity = new ChromaEntity;
		Scene->addEntity(SphereEntity);
		SphereEntity->setName("Sphere");
		ChromaMeshComponent* SphereMeshComponent = new SpherePrimitive();
		ChromaPhysicsComponent* SphereRigidComponent = new ChromaPhysicsComponent();
		SphereRigidComponent->setCollisionShape(ColliderShape::Convex);
		SphereRigidComponent->setCollisionState(ColliderState::Kinematic);
		//SphereMeshComponent->bindTexture(sandyNormal);
		SphereMeshComponent->bindTexture(greyAlbedo);
		SphereMeshComponent->bindShader(&PBRShader);
		SphereEntity->addComponent(SphereMeshComponent);
		SphereEntity->addComponent(SphereRigidComponent);
		SphereEntity->setPosition(position);
	}

	// LOOKDEV
	IChromaEntity* SphereEntityLookDev = new ChromaEntity;
	Scene->addEntity(SphereEntityLookDev);
	SphereEntityLookDev->setName("LookDev");
	ChromaMeshComponent* SphereLookDevMeshComponent = new Model("resources/assets/lookdev/sphere.obj");
	ChromaPhysicsComponent* SphereLookDevRigidComponent = new ChromaPhysicsComponent();
	SphereLookDevRigidComponent->setCollisionShape(ColliderShape::Sphere);
	SphereLookDevRigidComponent->setCollisionState(ColliderState::Kinematic);
	SphereLookDevMeshComponent->bindShader(&PBRShader);
	SphereLookDevMeshComponent->bindTexture(lookdevAlbedo);
	SphereLookDevMeshComponent->bindTexture(lookdevNormal);
	SphereLookDevMeshComponent->bindTexture(lookdevMetRoughAO);
	SphereEntityLookDev->addComponent(SphereLookDevMeshComponent);
	SphereEntityLookDev->addComponent(SphereLookDevRigidComponent);
	SphereEntityLookDev->setPosition(glm::vec3(0.0f, 2.0f, -4.0f));
	SphereEntityLookDev->setScale(glm::vec3(0.25));


	// RUSTED IRON
	IChromaEntity* SphereEntityRustedIron = new ChromaEntity;
	Scene->addEntity(SphereEntityRustedIron);
	SphereEntityRustedIron->setName("Rusted Iron");
	ChromaMeshComponent* SphereRustedIronMeshComponent = new Model("resources/assets/lookdev/sphere.obj");
	ChromaPhysicsComponent* SphereRustedIronRigidComponent = new ChromaPhysicsComponent();
	SphereRustedIronRigidComponent->setCollisionShape(ColliderShape::Sphere);
	SphereRustedIronRigidComponent->setCollisionState(ColliderState::Kinematic);
	SphereRustedIronMeshComponent->bindTexture(rustedIronAlbedo);
	SphereRustedIronMeshComponent->bindTexture(rustedIronNormal);
	SphereRustedIronMeshComponent->bindTexture(rustedIronMetRoughAO);
	SphereRustedIronMeshComponent->bindShader(&PBRShader);
	SphereEntityRustedIron->addComponent(SphereRustedIronMeshComponent);
	SphereEntityRustedIron->addComponent(SphereRustedIronRigidComponent);
	SphereEntityRustedIron->setPosition(glm::vec3(-2.5f, 1.0f, 0.0f));
	SphereEntityRustedIron->setScale(glm::vec3(0.15));

	// WOOD PLANKS
	IChromaEntity* SphereEntityWoodplanks = new ChromaEntity;
	Scene->addEntity(SphereEntityWoodplanks);
	SphereEntityWoodplanks->setName("Wood Planks");
	ChromaMeshComponent* SphereWoodplanksMeshComponent = new Model("resources/assets/lookdev/sphere.obj");
	ChromaPhysicsComponent* SpherewoodRigidComponent = new ChromaPhysicsComponent();
	SpherewoodRigidComponent->setCollisionShape(ColliderShape::Sphere);
	SpherewoodRigidComponent->setCollisionState(ColliderState::Kinematic);
	SphereWoodplanksMeshComponent->bindTexture(agedPlanksAlbedo);
	SphereWoodplanksMeshComponent->bindTexture(agedPlanksNormal);
	SphereWoodplanksMeshComponent->bindTexture(agedPlanksMetRoughAO);
	SphereWoodplanksMeshComponent->bindShader(&PBRShader);
	SphereEntityWoodplanks->addComponent(SphereWoodplanksMeshComponent);
	SphereEntityWoodplanks->addComponent(SpherewoodRigidComponent);
	SphereEntityWoodplanks->setPosition(glm::vec3(-5.f, 1.0f, 0.0f));
	SphereEntityWoodplanks->setScale(glm::vec3(0.15));


	// SEMI TRANSPARENT
	IChromaEntity* SphereEntityTransparent = new ChromaEntity;
	Scene->addEntity(SphereEntityTransparent);
	SphereEntityTransparent->setName("Sphere");
	ChromaMeshComponent* SphereMeshComponent = new SpherePrimitive;
	ChromaPhysicsComponent* SphereRigidComponent = new ChromaPhysicsComponent();
	SphereRigidComponent->setCollisionShape(ColliderShape::Convex);
	SphereRigidComponent->setCollisionState(ColliderState::Kinematic);
	SphereMeshComponent->bindTexture(alphaTestAlbedo);
	SphereMeshComponent->bindShader(&SemiTransparentShader);
	SphereMeshComponent->isLit = false;
	SphereMeshComponent->isForwardLit = true;
	SphereEntityTransparent->addComponent(SphereMeshComponent);
	SphereEntityTransparent->addComponent(SphereRigidComponent);
	SphereEntityTransparent->setPosition(glm::vec3(7.5, 1.0, 0.0));

	// UNLIT
	IChromaEntity* SphereEntityUnlit = new ChromaEntity;
	Scene->addEntity(SphereEntityUnlit);
	SphereEntityUnlit->setName("Sphere");
	ChromaMeshComponent* SphereMeshComponentUnlit = new SpherePrimitive;
	ChromaPhysicsComponent* SphereRigidComponentUnlit = new ChromaPhysicsComponent();
	SphereRigidComponentUnlit->setCollisionShape(ColliderShape::Convex);
	SphereRigidComponentUnlit->setCollisionState(ColliderState::Kinematic);
	SphereMeshComponentUnlit->bindShader(&UnlitShader);
	SphereMeshComponentUnlit->isLit = false;
	SphereMeshComponentUnlit->castShadows = false;
	SphereEntityUnlit->addComponent(SphereMeshComponentUnlit);
	SphereEntityUnlit->addComponent(SphereRigidComponentUnlit);
	SphereEntityUnlit->setPosition(glm::vec3(-7.5, 1.0, 0.0));

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

		//Sunlight Rotation		
		Sun->setPosition(glm::vec3(std::sin(GameTime* SUNLIGHT_SPIN_SPEED)* SUNLIGHT_DISTANCE, SUNLIGHT_DISTANCE, std::cos(GameTime* SUNLIGHT_SPIN_SPEED)* SUNLIGHT_DISTANCE));
		Sun->setDirection(-normalize(Sun->getPosition()));


		Game.getRenderer()->getDebugBuffer()->drawBox(glm::vec3(3), glm::vec3(5), glm::vec3(1,0,0));

		Game.getRenderer()->getDebugBuffer()->drawLine(glm::vec3(-3, 3, 3), glm::vec3(-5, 5, 5), glm::vec3(0, 0, 1));


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