// c++ standard libs
#include <iostream>
#include <vector>
#include <map>
//// glad and glfw
#include <glad/glad.h>
// glm
#include <glm/glm.hpp>

// chroma
#include <screenManager/ScreenManager.h>
#include <animation/AnimationLoader.h>
#include <component/IComponent.h>
#include <component/MeshComponent.h>
#include <component/AnimationComponent.h>
#include <entity/Entity.h>
#include <scene/Scene.h>
#include <model/Model.h>
#include <model/BoxPrimitive.h>
#include <model/SpherePrimitive.h>
#include <shader/Shader.h>
#include <texture/Texture.h>
#include <texture/HDRTexture.h>
#include <light/Light.h>
#include <terrain/Terrain.h>
#include <game/ChromaGame.h>
#include <time/Time.h>


int main()
{

	// INIT CHROMA
	// ------------------------------------------------------------------------------------------
	// SCENE 
	Scene* scene{ new Scene };

	// GAME
	ChromaGame Game(scene);

	// SCENE CONTENTS
	// ------------------------------------------------------------------------------------------

	// LIGHTS
	// ____________________________________________________
	const float SUNLIGHT_SPIN_SPEED = .8f;
	const float SUNLIGHT_DISTANCE = 15.0f;

	// POINT LIGHTS
	std::vector<Light*> Lights;

	// point light positions
	glm::vec3 pointLightPositions[] = {
		glm::vec3(3.5f,  1.2f,  2.0f),
		glm::vec3(0.5f,  0.2f,  -2.0f),
		glm::vec3(-3.5f,  1.2f,  4.0f),
	};
	// dancing point lights
	for (glm::vec3 pos : pointLightPositions)
	{
		Light* pointLight = new Light(pos, Light::POINT);
		pointLight->setIntensity(0.51f);
		pointLight->quadratic *= 4.0f;
		pointLight->linear *= 2.0f;
		Lights.push_back(pointLight);
	}

	// SUNLIGHT
	Light* Sun = new Light(Light::SUNLIGHT, glm::vec3(0.2, -0.8, 0.3), 2.0f);
	Sun->setDiffuse(glm::vec3(1.0));
	Sun->setIntensity(3.0);
	Lights.push_back(Sun);

	scene->SetLights(Lights);
	// ____________________________________________________

	// SHADERS
	// ____________________________________________________
	Shader UnlitShader("resources/shaders/fragBasic.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
	UnlitShader.use();
	UnlitShader.setVec3("color", glm::vec3(1, 1, 0));
	Shader SemiTransparentShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
	Shader PBRShader("resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");

	// Lookdev Sphere
	Texture lookdevAlbedo("resources/textures/pbr/lookdev_pbr/albedo.jpg");
	lookdevAlbedo.type = Texture::ALBEDO;
	Texture lookdevNormal("resources/textures/pbr/lookdev_pbr/normal.jpg");
	lookdevNormal.type = Texture::NORMAL;
	Texture lookdevMetRoughAO("resources/textures/pbr/lookdev_pbr/MetRoughAO.jpg");
	lookdevMetRoughAO.type = Texture::METROUGHAO;
	// ____________________________________________________

	// TEXTURES
	// ____________________________________________________
	// Generic
	Texture blackAlbedo("resources/textures/colors/black.jpg");
	Texture greyAlbedo("resources/textures/colors/grey.jpg");
	Texture whiteAlbedo("resources/textures/colors/white.jpg");
	Texture alphaTestAlbedo("resources/textures/test/alphatest.png");
	Texture sandyNormal("resources/textures/test/sandy_normal.jpg");
	sandyNormal.type = Texture::NORMAL;

	// Woodplanks
	Texture PlanksAlbedo("resources/textures/pbr/hardwood_pbr/albedo.jpg");
	PlanksAlbedo.type = Texture::ALBEDO;
	Texture PlanksNormal("resources/textures/pbr/hardwood_pbr/normal.jpg");
	PlanksNormal.type = Texture::NORMAL;
	Texture PlanksMetRoughAO("resources/textures/pbr/hardwood_pbr/MetRoughAO.jpg");
	PlanksMetRoughAO.type = Texture::METROUGHAO;

	// AgedWoodplanks
	Texture agedPlanksAlbedo("resources/textures/pbr/agedplanks_pbr/albedo.jpg");
	agedPlanksAlbedo.type = Texture::ALBEDO;
	Texture agedPlanksNormal("resources/textures/pbr/agedplanks_pbr/normal.jpg");
	agedPlanksNormal.type = Texture::NORMAL;
	Texture agedPlanksMetRoughAO("resources/textures/pbr/agedplanks_pbr/MetRoughAO.jpg");
	agedPlanksMetRoughAO.type = Texture::METROUGHAO;

	// Rusted Metal
	Texture rustedIronAlbedo("resources/textures/pbr/rustediron_pbr/albedo.jpg");
	rustedIronAlbedo.type = Texture::ALBEDO;
	Texture rustedIronNormal("resources/textures/pbr/rustediron_pbr/normal.jpg");
	rustedIronNormal.type = Texture::NORMAL;
	Texture rustedIronMetRoughAO("resources/textures/pbr/rustediron_pbr/MetRoughAO.jpg");
	rustedIronMetRoughAO.type = Texture::METROUGHAO;

	// Animated Model
	Texture walkingAlbedo("resources/animation/walking_textures/albedo.jpg");
	walkingAlbedo.type = Texture::ALBEDO;
	Texture walkingNormal("resources/animation/walking_textures/normal.jpg");
	walkingNormal.type = Texture::NORMAL;
	Texture walkingMetRoughAO("resources/animation/walking_textures/MetRoughAO.jpg");
	walkingMetRoughAO.type = Texture::METROUGHAO;
	// ____________________________________________________

	// ANIMATED MODEL
	// ____________________________________________________
	//IEntity* AnimModelEntity = new Entity;
	//AnimModelEntity->SetName("AnimationModel");
	//scene->AddEntity(AnimModelEntity);
	//// mesh
	//MeshComponent* AnimModelMeshComponent = new Model("resources/animation/walking8.fbx");
	//AnimModelMeshComponent->SetShader(&PBRShader);
	//AnimModelMeshComponent->AddTexture(walkingAlbedo);
	//AnimModelMeshComponent->AddTexture(walkingNormal);
	//AnimModelMeshComponent->AddTexture(walkingMetRoughAO);
	//AnimModelEntity->AddComponent(AnimModelMeshComponent);
	//// rigid
	//PhysicsComponent* AnimModelRigidComponent = new PhysicsComponent();
	//AnimModelRigidComponent->SetColliderShape(ColliderShape::Box);
	//AnimModelRigidComponent->SetCollisionState(ColliderState::Kinematic);
	//AnimModelEntity->AddComponent(AnimModelRigidComponent);
	//// animation
	//AnimationComponent* AnimModelAnimationComponent = new AnimationComponent();
	//Animator AnimModelAnimator;
	//AnimModelAnimator.LoadAnimations("resources/animation/walking.fbx");
	//AnimModelAnimator.BindSkeleton(((Model*)AnimModelMeshComponent)->GetSkeleton());
	//AnimModelAnimationComponent->AddAnimator(AnimModelAnimator);
	//AnimModelEntity->AddComponent(AnimModelAnimationComponent);
	//// transforming entity
	//////((Model*)AnimModelMeshComponent)->SetTranslation(glm::vec3(-2, 0, -4));
	//((Model*)AnimModelMeshComponent)->SetScale(0.06);
	// ____________________________________________________


	// CAPSULE
	// ____________________________________________________
	IEntity* CapsuleEntity = new Entity;
	scene->AddEntity(CapsuleEntity);
	// mesh
	MeshComponent* CapsuleMeshComponent = new Model("resources/primitives/capsule.fbx");
	CapsuleMeshComponent->SetShader(&PBRShader);
	CapsuleMeshComponent->AddTexture(greyAlbedo);
	CapsuleEntity->AddComponent(CapsuleMeshComponent);
	// rigid
	PhysicsComponent* CapsuleRigidComponent = new PhysicsComponent();
	CapsuleRigidComponent->SetColliderShape(ColliderShape::Box);
	CapsuleRigidComponent->SetCollisionState(ColliderState::Kinematic);
	CapsuleEntity->AddComponent(CapsuleRigidComponent);
	// transform
	CapsuleEntity->SetPosition(glm::vec3(0,1.5,0.0));
	// character controller


	// ____________________________________________________


	// TERRAIN
	// ____________________________________________________
	IEntity* TerrainEntity = new Entity;
	scene->AddEntity(TerrainEntity);
	MeshComponent* TerrainMeshComponent = new Terrain;
	TerrainMeshComponent->SetShader(&PBRShader);
	TerrainMeshComponent->AddTexture(PlanksAlbedo);
	TerrainMeshComponent->AddTexture(PlanksNormal);
	TerrainMeshComponent->AddTexture(PlanksMetRoughAO);
	TerrainEntity->AddComponent(TerrainMeshComponent);
	// ____________________________________________________

	//// SPHERES
	//// Sphere Positions
	//glm::vec3 spherePositions[] = {
	//	glm::vec3(0.f,  1.0f,  0.0f),
	//	glm::vec3(2.5f,  1.0f,  0.0f),
	//	glm::vec3(5.0f,  1.0f,  0.0f)
	//};

	//
	//for (int i =0; i < 3; i++ )
	//{
	//	IEntity* SphereEntity = new Entity;
	//	scene->AddEntity(SphereEntity);
	//	SphereEntity->SetName("Sphere");
	//	MeshComponent* SphereMeshComponent = new SpherePrimitive();
	//	PhysicsComponent* SphereRigidComponent = new PhysicsComponent();
	//	SphereRigidComponent->SetColliderShape(ColliderShape::Box);
	//	SphereRigidComponent->SetCollisionState(ColliderState::Kinematic);
	//	//SphereMeshComponent->AddTexture(sandyNormal);
	//	SphereMeshComponent->AddTexture(greyAlbedo);
	//	SphereMeshComponent->SetShader(&PBRShader);
	//	SphereEntity->SetPosition(spherePositions[i]);
	//	SphereEntity->AddComponent(SphereMeshComponent);
	//	SphereEntity->AddComponent(SphereRigidComponent);
	//}
	//// ____________________________________________________
	//
	//// CUBES
	//// ____________________________________________________
	//IEntity* CubeEntity = new Entity;
	//scene->AddEntity(CubeEntity);
	//CubeEntity->SetName("Cube");
	//MeshComponent* CubeMeshComponent = new BoxPrimitive();
	//PhysicsComponent* CubeRigidComponent = new PhysicsComponent();
	//CubeRigidComponent->SetColliderShape(ColliderShape::Box);
	//CubeRigidComponent->SetCollisionState(ColliderState::Kinematic);
	////SphereMeshComponent->AddTexture(sandyNormal);
	//CubeMeshComponent->AddTexture(greyAlbedo);
	//CubeMeshComponent->SetShader(&PBRShader);
	//CubeEntity->AddComponent(CubeMeshComponent);
	//CubeEntity->AddComponent(CubeRigidComponent);
	//CubeEntity->SetPosition(glm::vec3(-5.0f, 1.0f, 0.0f));
	//// ____________________________________________________

	//
	//// ____________________________________________________

	////LOOKDEV
	//IEntity* SphereEntityLookDev = new Entity;
	//scene->AddEntity(SphereEntityLookDev);
	//SphereEntityLookDev->SetName("LookDev");
	//MeshComponent* SphereLookDevMeshComponent = new Model("resources/assets/lookdev/sphere.obj");
	//PhysicsComponent* SphereLookDevRigidComponent = new PhysicsComponent();
	//SphereLookDevRigidComponent->SetColliderShape(ColliderShape::Box);
	//SphereLookDevRigidComponent->SetCollisionState(ColliderState::Kinematic);
	//SphereLookDevMeshComponent->SetShader(&PBRShader);
	//SphereLookDevMeshComponent->AddTexture(lookdevAlbedo);
	//SphereLookDevMeshComponent->AddTexture(lookdevNormal);
	//SphereLookDevMeshComponent->AddTexture(lookdevMetRoughAO);
	//SphereEntityLookDev->SetPosition(glm::vec3(2.0f, 2.0f, -4.0f));
	//SphereEntityLookDev->setScale(glm::vec3(0.25));
	//SphereEntityLookDev->AddComponent(SphereLookDevMeshComponent);
	//SphereEntityLookDev->AddComponent(SphereLookDevRigidComponent);

	// //RUSTED IRON
	//IEntity* SphereEntityRustedIron = new Entity;
	//scene->AddEntity(SphereEntityRustedIron);
	//SphereEntityRustedIron->SetName("Rusted Iron");
	//MeshComponent* SphereRustedIronMeshComponent = new Model("resources/assets/lookdev/sphere.obj");
	//PhysicsComponent* SphereRustedIronRigidComponent = new PhysicsComponent();
	//SphereRustedIronRigidComponent->SetColliderShape(ColliderShape::Sphere);
	//SphereRustedIronRigidComponent->SetCollisionState(ColliderState::Kinematic);
	//SphereRustedIronMeshComponent->AddTexture(rustedIronAlbedo);
	//SphereRustedIronMeshComponent->AddTexture(rustedIronNormal);
	//SphereRustedIronMeshComponent->AddTexture(rustedIronMetRoughAO);
	//SphereRustedIronMeshComponent->SetShader(&PBRShader);
	//SphereEntityRustedIron->setScale(glm::vec3(0.15));
	//SphereEntityRustedIron->SetPosition(glm::vec3(-2.5f, 1.0f, 0.0f));
	//SphereEntityRustedIron->AddComponent(SphereRustedIronMeshComponent);
	//SphereEntityRustedIron->AddComponent(SphereRustedIronRigidComponent);

	//// WOOD PLANKS
	//IEntity* SphereEntityWoodplanks = new Entity;
	//scene->AddEntity(SphereEntityWoodplanks);
	//SphereEntityWoodplanks->SetName("Wood Planks");
	//MeshComponent* SphereWoodplanksMeshComponent = new Model("resources/assets/lookdev/sphere.obj");
	//PhysicsComponent* SpherewoodRigidComponent = new PhysicsComponent();
	//SpherewoodRigidComponent->SetColliderShape(ColliderShape::Sphere);
	//SpherewoodRigidComponent->SetCollisionState(ColliderState::Kinematic);
	//SphereWoodplanksMeshComponent->AddTexture(agedPlanksAlbedo);
	//SphereWoodplanksMeshComponent->AddTexture(agedPlanksNormal);
	//SphereWoodplanksMeshComponent->AddTexture(agedPlanksMetRoughAO);
	//SphereWoodplanksMeshComponent->SetShader(&PBRShader);
	//SphereEntityWoodplanks->SetPosition(glm::vec3(-5.f, 1.0f, 0.0f));
	//SphereEntityWoodplanks->setScale(glm::vec3(0.15));
	//SphereEntityWoodplanks->AddComponent(SphereWoodplanksMeshComponent);
	//SphereEntityWoodplanks->AddComponent(SpherewoodRigidComponent);

	//// SEMI TRANSPARENT
	//IEntity* SphereEntityTransparent = new Entity;
	//scene->AddEntity(SphereEntityTransparent);
	//SphereEntityTransparent->SetName("Sphere");
	//MeshComponent* SphereMeshComponent = new SpherePrimitive;
	//PhysicsComponent* SphereRigidComponent = new PhysicsComponent();
	//SphereRigidComponent->SetColliderShape(ColliderShape::Sphere);
	//SphereRigidComponent->SetCollisionState(ColliderState::Kinematic);
	//SphereMeshComponent->AddTexture(alphaTestAlbedo);
	//SphereMeshComponent->SetShader(&PBRShader);
	//SphereMeshComponent->m_IsLit = true;
	////SphereMeshComponent->m_IsForwardLit = true;
	////SphereMeshComponent->m_IsTransparent = true;
	//SphereEntityTransparent->SetPosition(glm::vec3(7.5, 1.0, 0.0));
	//SphereEntityTransparent->AddComponent(SphereMeshComponent);
	//SphereEntityTransparent->AddComponent(SphereRigidComponent);

	//// UNLIT
	//IEntity* SphereEntityUnlit = new Entity;
	//scene->AddEntity(SphereEntityUnlit);
	//SphereEntityUnlit->SetName("Sphere");
	//MeshComponent* SphereMeshComponentUnlit = new SpherePrimitive;
	//PhysicsComponent* SphereRigidComponentUnlit = new PhysicsComponent();
	//SphereRigidComponentUnlit->SetColliderShape(ColliderShape::Convex);
	//SphereRigidComponentUnlit->SetCollisionState(ColliderState::Kinematic);
	//SphereMeshComponentUnlit->SetShader(&UnlitShader);
	//SphereMeshComponentUnlit->m_IsLit = false;
	//SphereMeshComponentUnlit->m_CastShadows = false;
	//SphereEntityUnlit->SetPosition(glm::vec3(-7.5, 1.0, 0.0));
	//SphereEntityUnlit->AddComponent(SphereMeshComponentUnlit);
	//SphereEntityUnlit->AddComponent(SphereRigidComponentUnlit);
	//// ____________________________________________________


	// RENDER LOOP
	// -----------
	while (!Chroma::Screen::GetShouldClose())
	{
		// SCREENMANAGER START
		float GameTime = Chroma::Time::GetGameTime();
		float DeltaTime = Chroma::Time::GetDeltaTime();

		//Sunlight Rotation		
		Sun->SetPosition(glm::vec3(std::sin(GameTime* SUNLIGHT_SPIN_SPEED)* SUNLIGHT_DISTANCE, SUNLIGHT_DISTANCE, std::cos(GameTime* SUNLIGHT_SPIN_SPEED)* SUNLIGHT_DISTANCE));
		Sun->setDirection(-normalize(Sun->GetPosition()));

		//CubeEntity->SetPosition(glm::vec3(-5.0f, glm::sin(GameTime) * 3.0, 0.0f));

		Game.GetRenderer()->GetDebugBuffer()->DrawBox(glm::vec3(3), glm::vec3(5), glm::vec3(1,0,0));
		//Game.getRenderer()->getDebugBuffer()->drawLine(glm::vec3(-3, 3, 3), glm::vec3(-5, 5, 5), glm::vec3(0, 0, 1));
		//((Model*)AnimModelMeshComponent)->SetTranslation(glm::vec3(-2, glm::sin(GameTime) * 20, 4));
		glm::vec3 rotationAxis{ 0.0, 1.0, 0.0 };
		float rotationAmount = glm::radians(glm::sin(GameTime*0.5) * 90);
		//((Model*)AnimModelMeshComponent)->SetRotation(glm::angleAxis(rotationAmount, rotationAxis));
		// GAME TICK
		Game.Tick();

		// END SCREEN
		Chroma::Screen::Update();
	}


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	Chroma::Screen::Close();
	return 0;
}