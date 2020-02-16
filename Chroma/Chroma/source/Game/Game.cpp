// c++ standard libs
#include <vector>

// glm
#include <glm/glm.hpp>

// chroma
#include <engine/engine.h>

#include <entity/Entity.h>
#include <terrain/Terrain.h>
#include <component/AnimationComponent.h>
#include <model/SkinnedMesh.h>

// game
#include <thirdperson/ThirdPersonCharacterController.h>
#include <thirdperson/ThirdPersonCameraController.h>

int main2()
{
	// INIT CHROMA
	Chroma::Engine::Init();

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

	Chroma::Scene::SetLights(Lights);
	// ____________________________________________________
	// SHADERS
	// ____________________________________________________
	Shader UnlitShader("resources/shaders/fragBasic.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
	UnlitShader.use();
	UnlitShader.SetVec3("color", glm::vec3(1, 1, 0));
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
	Texture gridAlbedo("resources/animation/textures/grid.jpg");
	Texture flatNormal("resources/textures/test/flat_normal.jpg");
	flatNormal.type = Texture::NORMAL;
	Texture alphaTestAlbedo("resources/textures/test/grass.png");
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
	Texture walkingAlbedo("resources/animation/vampire_textures/albedo.jpg");
	walkingAlbedo.type = Texture::ALBEDO;
	Texture walkingNormal("resources/animation/vampire_textures/normal.jpg");
	walkingNormal.type = Texture::NORMAL;
	Texture walkingMetRoughAO("resources/animation/vampire_textures/MetRoughAO.jpg");
	walkingMetRoughAO.type = Texture::METROUGHAO;
	// ____________________________________________________

	// ANIMATED MODEL
	// ____________________________________________________
	IEntity* AnimModelEntity = new Entity; 
	AnimModelEntity->SetName("AnimationModel");
	Chroma::Scene::AddEntity(AnimModelEntity);
	AnimModelEntity->SetScale(glm::vec3(0.06));
	AnimModelEntity->SetTranslation(glm::vec3(0.0, 10.0,0.0));
	// mesh
	MeshComponent* AnimModelMeshComponent = new SkinnedMesh("resources/animation/vampire.fbx");
	AnimModelMeshComponent->AddTexture(walkingAlbedo);
	AnimModelMeshComponent->AddTexture(walkingNormal);
	AnimModelMeshComponent->AddTexture(walkingMetRoughAO);
	AnimModelMeshComponent->SetIsDoubleSided(true);
	AnimModelEntity->AddComponent(AnimModelMeshComponent);
	// rigid
	PhysicsComponent* AnimModelRigidComponent = new PhysicsComponent();
	AnimModelRigidComponent->SetColliderShape(ColliderShape::Box);
	AnimModelRigidComponent->SetCollisionState(ColliderState::Kinematic);
	AnimModelEntity->AddComponent(AnimModelRigidComponent);
	// animation
	AnimationComponent* AnimModelAnimationComponent = new AnimationComponent();
	Animator AnimModelAnimator;
	AnimModelAnimator.BindSkeleton(AnimModelMeshComponent);
	AnimModelAnimator.LoadAnimations("resources/animation/vampire_idle.fbx");
	AnimModelAnimator.LoadAnimations("resources/animation/vampire_walk.fbx");
	AnimModelAnimator.CompressAnimations();
	AnimModelAnimationComponent->AddAnimator(AnimModelAnimator);
	AnimModelEntity->AddComponent(AnimModelAnimationComponent);
	//// character controller
	CharacterControllerComponent* AnimModelCharacterController = new ThirdPersonCharacterController();
	// camera controller
	ICameraController* AnimModelCameraController = new ThirdPersonCameraController();
	Chroma::Scene::GetRenderCamera()->SetCustomCameraController(AnimModelCameraController);
	AnimModelCharacterController->SetCustomCameraController(AnimModelCameraController);
	// adding the component
	AnimModelEntity->AddComponent(AnimModelCharacterController);

	// ____________________________________________________


	//// CAPSULE
	//// ____________________________________________________
	//IEntity* CapsuleEntity = new Entity;
	//Chroma::Scene::AddEntity(CapsuleEntity);
	//// mesh
	//MeshComponent* CapsuleMeshComponent = new Model("resources/primitives/capsule.fbx");
	//CapsuleMeshComponent->SetShader(&PBRShader);
	//CapsuleMeshComponent->AddTexture(walkingAlbedo);
	//CapsuleMeshComponent->AddTexture(walkingNormal);
	//CapsuleMeshComponent->AddTexture(walkingMetRoughAO);
	////CapsuleMeshComponent->SetShader(&PBRShader);
	////CapsuleMeshComponent->AddTexture(greyAlbedo);
	//CapsuleEntity->AddComponent(CapsuleMeshComponent);
	//// rigid
	//PhysicsComponent* CapsuleRigidComponent = new PhysicsComponent();
	//CapsuleRigidComponent->SetColliderShape(ColliderShape::Box);
	//CapsuleRigidComponent->SetCollisionState(ColliderState::Kinematic);
	//CapsuleEntity->AddComponent(CapsuleRigidComponent);
	//// transform
	//CapsuleEntity->SetTranslation(glm::vec3(0,1.75,0.0));
	//// character controller
	//CharacterControllerComponent* CapsuleCharacterController = new ThirdPersonCharacterController();
	//// camera controller
	//ICameraController* CapsuleCameraController = new ThirdPersonCameraController();
	//Chroma::Scene::GetRenderCamera()->SetCustomCameraController(CapsuleCameraController);
	//CapsuleCharacterController->SetCustomCameraController(CapsuleCameraController);
	//// adding the component
	//CapsuleEntity->AddComponent(CapsuleCharacterController);
	// ____________________________________________________


	// TERRAIN
	// ____________________________________________________
	IEntity* TerrainEntity = new Entity;
	Chroma::Scene::AddEntity(TerrainEntity);
	MeshComponent* TerrainMeshComponent = new Terrain;
	TerrainMeshComponent->SetShader(&PBRShader);
	TerrainMeshComponent->AddTexture(gridAlbedo);
	TerrainMeshComponent->AddTexture(flatNormal);
	TerrainMeshComponent->m_UVMultiply = glm::vec2(8.0f);
	//TerrainMeshComponent->AddTexture(PlanksNormal);
	//TerrainMeshComponent->AddTexture(PlanksMetRoughAO);
	TerrainEntity->AddComponent(TerrainMeshComponent);
	TerrainEntity->SetScale(glm::vec3(10.0, 1.0, 10.0));
	// ____________________________________________________

	// SPHERES
	// Sphere Positions
	glm::vec3 spherePositions[] = {
		glm::vec3(0.f,  1.0f,  0.0f),
		glm::vec3(2.5f,  1.0f,  0.0f),
		glm::vec3(5.0f,  1.0f,  0.0f),
		glm::vec3(-2.5f,  1.0f,  0.0f),
		glm::vec3(-5.0f,  1.0f,  0.0f),
	};

	
	for (int i =0; i < 5; i++ )
	{
		IEntity* SphereEntity = new Entity;
		Chroma::Scene::AddEntity(SphereEntity);
		SphereEntity->SetName("Sphere");
		MeshComponent* SphereMeshComponent = new SpherePrimitive();
		PhysicsComponent* SphereRigidComponent = new PhysicsComponent();
		SphereRigidComponent->SetColliderShape(ColliderShape::Convex);
		SphereRigidComponent->SetCollisionState(ColliderState::Dynamic);
		SphereRigidComponent->SetMass(1.0f);
		SphereRigidComponent->SetFriction(3.0f);
		//SphereMeshComponent->AddTexture(sandyNormal);
		SphereMeshComponent->AddTexture(greyAlbedo);
		SphereMeshComponent->SetShader(&PBRShader);
		SphereEntity->SetTranslation(spherePositions[i]);
		SphereEntity->AddComponent(SphereMeshComponent);
		SphereEntity->AddComponent(SphereRigidComponent);
	}
	// ____________________________________________________
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
	//CubeEntity->SetTranslation(glm::vec3(-5.0f, 1.0f, 0.0f));
	//// ____________________________________________________

	//
	//// ____________________________________________________

	//LOOKDEV
	//IEntity* SphereEntityLookDev = new Entity;
	//Chroma::Scene::AddEntity(SphereEntityLookDev);
	//SphereEntityLookDev->SetName("LookDev");
	//MeshComponent* SphereLookDevMeshComponent = new Model("resources/assets/lookdev/sphere.obj");
	//PhysicsComponent* SphereLookDevRigidComponent = new PhysicsComponent();
	//SphereLookDevRigidComponent->SetColliderShape(ColliderShape::Box);
	//SphereLookDevRigidComponent->SetCollisionState(ColliderState::Kinematic);
	//SphereLookDevMeshComponent->SetShader(&PBRShader);
	//SphereLookDevMeshComponent->AddTexture(lookdevAlbedo);
	//SphereLookDevMeshComponent->AddTexture(lookdevNormal);
	//SphereLookDevMeshComponent->AddTexture(lookdevMetRoughAO);
	//SphereEntityLookDev->SetTranslation(glm::vec3(2.0f, 2.0f, -4.0f));
	//SphereEntityLookDev->SetScale(glm::vec3(0.25));
	//SphereEntityLookDev->AddComponent(SphereLookDevMeshComponent);
	//SphereEntityLookDev->AddComponent(SphereLookDevRigidComponent);

	 //RUSTED IRON
	//IEntity* SphereEntityRustedIron = new Entity;
	//Chroma::Scene::AddEntity(SphereEntityRustedIron);
	//SphereEntityRustedIron->SetName("Rusted Iron");
	//MeshComponent* SphereRustedIronMeshComponent = new Model("resources/assets/lookdev/sphere.obj");
	//PhysicsComponent* SphereRustedIronRigidComponent = new PhysicsComponent();
	//SphereRustedIronRigidComponent->SetColliderShape(ColliderShape::Sphere);
	//SphereRustedIronRigidComponent->SetCollisionState(ColliderState::Kinematic);
	////SphereRustedIronRigidComponent->SetMass(1.0f);
	//SphereRustedIronMeshComponent->AddTexture(rustedIronAlbedo);
	//SphereRustedIronMeshComponent->AddTexture(rustedIronNormal);
	//SphereRustedIronMeshComponent->AddTexture(rustedIronMetRoughAO);
	//SphereRustedIronMeshComponent->SetShader(&PBRShader);
	//SphereEntityRustedIron->SetTranslation(glm::vec3(-2.5f, 1.0f, 0.0f));
	//SphereEntityRustedIron->SetScale(glm::vec3(0.15));
	//SphereEntityRustedIron->AddComponent(SphereRustedIronMeshComponent);
	//SphereEntityRustedIron->AddComponent(SphereRustedIronRigidComponent);

	// WOOD PLANKS
	//IEntity* SphereEntityWoodplanks = new Entity;
	//Chroma::Scene::AddEntity(SphereEntityWoodplanks);
	//SphereEntityWoodplanks->SetName("Wood Planks");
	//MeshComponent* SphereWoodplanksMeshComponent = new Model("resources/assets/lookdev/sphere.obj");
	//PhysicsComponent* SpherewoodRigidComponent = new PhysicsComponent();
	//SpherewoodRigidComponent->SetColliderShape(ColliderShape::Sphere);
	//SpherewoodRigidComponent->SetCollisionState(ColliderState::Kinematic);
	//SphereWoodplanksMeshComponent->AddTexture(agedPlanksAlbedo);
	//SphereWoodplanksMeshComponent->AddTexture(agedPlanksNormal);
	//SphereWoodplanksMeshComponent->AddTexture(agedPlanksMetRoughAO);
	//SphereWoodplanksMeshComponent->SetShader(&PBRShader);
	//SphereEntityWoodplanks->SetTranslation(glm::vec3(-5.f, 1.0f, 0.0f));
	//SphereEntityWoodplanks->SetScale(glm::vec3(0.15));
	//SphereEntityWoodplanks->AddComponent(SphereWoodplanksMeshComponent);
	//SphereEntityWoodplanks->AddComponent(SpherewoodRigidComponent);

	// SEMI TRANSPARENT
	//IEntity* GrassPlanesEntity = new Entity;
	//GrassPlanesEntity->SetTranslation(glm::vec3(0.0,0.0,-3.0));
	//GrassPlanesEntity->SetScale(glm::vec3(1.5));
	//Chroma::Scene::AddEntity(GrassPlanesEntity);
	//GrassPlanesEntity->SetName("Semi Transparent");
	//MeshComponent* GrassPlanesMeshComponent = new Model("resources/primitives/grassPlanes.fbx");
	//GrassPlanesMeshComponent->AddTexture(alphaTestAlbedo);
	//GrassPlanesMeshComponent->SetShader(&SemiTransparentShader);
	//GrassPlanesMeshComponent->m_IsLit = false;
	//GrassPlanesMeshComponent->m_IsForwardLit = true;
	//GrassPlanesMeshComponent->m_IsTransparent = true;
	//GrassPlanesEntity->AddComponent(GrassPlanesMeshComponent);

	//// SEMI TRANSPARENT
	//IEntity* SphereEntityTransparent2 = new Entity;
	//SphereEntityTransparent2->SetTranslation(glm::vec3(3.5, 20.0, 0.0));
	//Chroma::Scene::AddEntity(SphereEntityTransparent2);
	//SphereEntityTransparent2->SetName("Sphere Semi Transparent2");
	//MeshComponent* SphereMeshComponent2 = new SpherePrimitive;
	//PhysicsComponent* SphereRigidComponent2 = new PhysicsComponent();
	//SphereRigidComponent2->SetColliderShape(ColliderShape::Convex);
	//SphereRigidComponent2->SetCollisionState(ColliderState::Dynamic);
	//SphereRigidComponent2->SetMass(1.0f);
	//SphereMeshComponent2->AddTexture(alphaTestAlbedo);
	//SphereMeshComponent2->SetShader(&SemiTransparentShader);
	//SphereMeshComponent2->m_IsLit = false;
	//SphereMeshComponent2->m_IsTransparent = true;
	//SphereEntityTransparent2->AddComponent(SphereMeshComponent2);
	//SphereEntityTransparent2->AddComponent(SphereRigidComponent2);

	// UNLIT
	//IEntity* SphereEntityUnlit = new Entity;
	//SphereEntityUnlit->SetTranslation(glm::vec3(-7.5, 0.0, 0.0));
	//Chroma::Scene::AddEntity(SphereEntityUnlit);
	//SphereEntityUnlit->SetName("Sphere");
	//MeshComponent* SphereMeshComponentUnlit = new SpherePrimitive;
	//PhysicsComponent* SphereRigidComponentUnlit = new PhysicsComponent();
	//SphereRigidComponentUnlit->SetColliderShape(ColliderShape::Convex);
	//SphereRigidComponentUnlit->SetCollisionState(ColliderState::Kinematic);
	////SphereRigidComponentUnlit->SetMass(1.0f);
	//SphereRigidComponentUnlit->SetFriction(3.0f);
	//SphereMeshComponentUnlit->SetShader(&UnlitShader);
	//SphereMeshComponentUnlit->m_IsLit = false;
	//SphereMeshComponentUnlit->m_CastShadows = false;
	//SphereEntityUnlit->AddComponent(SphereMeshComponentUnlit);
	//SphereEntityUnlit->AddComponent(SphereRigidComponentUnlit);
	 //____________________________________________________

	Chroma::Scene::PostSceneBuild();

	// RENDER LOOP
	// -----------
	while (Chroma::Screen::IsRunning())
	{
		// SCREENMANAGER START
		double GameTime = Chroma::Time::GetGameTime();
		double DeltaTime = Chroma::Time::GetDeltaTime();

		//Sunlight Rotation	
		Chroma::Scene::GetSunLight()->SetTranslation(glm::vec3(std::sin(GameTime* SUNLIGHT_SPIN_SPEED)* SUNLIGHT_DISTANCE, SUNLIGHT_DISTANCE, std::cos(GameTime* SUNLIGHT_SPIN_SPEED)* SUNLIGHT_DISTANCE));
		Sun->setDirection(-normalize(Sun->GetPosition()));		

		//((Model*)AnimModelMeshComponent)->SetTranslation(glm::vec3(-2, glm::sin(GameTime) * 20, 4));
		glm::vec3 rotationAxis{ 0.0, 1.0, 0.0 };
		float rotationAmount = glm::radians(glm::sin(GameTime*0.5) * 90);
		//((Model*)AnimModelMeshComponent)->SetRotation(glm::angleAxis(rotationAmount, rotationAxis));
		

		// GAME TICK
		Chroma::Engine::Tick();

	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	Chroma::Screen::Close();
	return 0;
}