//common
#include <common/PrecompiledHeader.h>

// chroma
#include <engine/engine.h>
#include <entity/Entity.h>
#include <terrain/Terrain.h>
#include <component/AnimationComponent.h>
#include <component/CharacterPhysicsComponent.h>
#include <model/SkinnedMesh.h>
#include <bipedal/BipedalAnimationStateMachine.h>
#include <component/animation/IKAnimConstraint.h>

// game
#include <thirdperson/ThirdPersonCharacterController.h>
#include <thirdperson/ThirdPersonCameraController.h>

int main()
{
	// INIT CHROMA
	Chroma::Engine::Init();

	// Initialize Scene
	Chroma::Scene::PreSceneBuild();

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
		pointLight->m_Quadratic *= 4.0f;
		pointLight->m_Linear *= 2.0f;
		Lights.push_back(pointLight);
	}

	// SUNLIGHT
	Light* Sun = new Light(Light::SUNLIGHT, glm::vec3(0.2, -0.8, 0.3), 2.0f);
	Sun->setDiffuse(glm::vec3(1.0));
	Sun->setIntensity(3.0);
	Lights.push_back(Sun);
	Chroma::Scene::SetLights(Lights);

	// BACKGROUND
	//Chroma::Scene::GetIBL()->LoadIBL("resources/textures/ibl/grey_ibl/grey.hdr");

	// ____________________________________________________
	// SHADERS
	// ____________________________________________________
	Shader UnlitShader("resources/shaders/fragBasic.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
	UnlitShader.Use();
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

	// Animated Model
	Texture walkingAlbedo("resources/animation/vampire_textures/albedo.jpg");
	walkingAlbedo.type = Texture::ALBEDO;
	Texture walkingNormal("resources/animation/vampire_textures/normal.jpg");
	walkingNormal.type = Texture::NORMAL;
	Texture walkingMetRoughAO("resources/animation/vampire_textures/MetRoughAO.jpg");
	walkingMetRoughAO.type = Texture::METROUGHAO;

	// Floor Panels
	Texture woodBoardsAlbedo("resources/textures/pbr/hardwood_pbr/albedo.jpg");
	woodBoardsAlbedo.type = Texture::ALBEDO;
	Texture woodBoardsNormal("resources/textures/pbr/hardwood_pbr/normal.jpg");
	woodBoardsNormal.type = Texture::NORMAL;
	Texture woodBoardsMetRoughAO("resources/textures/pbr/hardwood_pbr/MetRoughAO.jpg");
	woodBoardsMetRoughAO.type = Texture::METROUGHAO;
	// ____________________________________________________

	// ANIMATED MODEL
	// ____________________________________________________
	IEntity* AnimModelEntity = new Entity;
	AnimModelEntity->SetName("AnimationModel");
	Chroma::Scene::AddEntity(AnimModelEntity);
	AnimModelEntity->SetScale(glm::vec3(0.03f));
	//AnimModelEntity->SetTranslation(glm::vec3(0.0, 5.0, 0.0));
	// mesh
	MeshComponent* AnimModelMeshComponent = new SkinnedMesh("resources/animation/vampire.fbx");
	AnimModelMeshComponent->AddTexture(walkingAlbedo);
	AnimModelMeshComponent->AddTexture(walkingNormal);
	AnimModelMeshComponent->AddTexture(walkingMetRoughAO);
	AnimModelMeshComponent->SetIsDoubleSided(true);
	AnimModelEntity->AddComponent(AnimModelMeshComponent);
	// rigid
	PhysicsComponent* AnimModelRigidComponent = new CharacterPhysicsComponent();
	AnimModelRigidComponent->SetColliderShape(ColliderShape::Capsule);
	AnimModelRigidComponent->SetCollisionState(ColliderState::Kinematic);
	AnimModelEntity->AddComponent(AnimModelRigidComponent);
	// animation
	AnimationComponent* AnimModelAnimationComponent = new AnimationComponent();
	Animator AnimModelAnimator;
	AnimModelAnimator.BindSkeleton(AnimModelMeshComponent);
	AnimModelAnimator.LoadAnimations("resources/animation/locomotion/Walk.fbx");
	AnimModelAnimator.LoadAnimations("resources/animation/locomotion/Idle.fbx");
	AnimModelAnimator.LoadAnimations("resources/animation/locomotion/Run.fbx");
	AnimModelAnimator.LoadAnimations("resources/animation/locomotion/Jump.fbx");
	AnimModelAnimator.LoadAnimations("resources/animation/locomotion/Roll.fbx");
	AnimModelAnimator.CompressAnimations();
	AnimationStateMachine* AnimModelAnimationStateMachine = new BipedalAnimationStateMachine;
	// animation state
	AnimModelAnimationComponent->SetAnimator(AnimModelAnimator);
	AnimModelAnimationComponent->SetAnimationStateMachine(AnimModelAnimationStateMachine);
	AnimModelEntity->AddComponent(AnimModelAnimationComponent);
	//// character controller
	CharacterControllerComponent* AnimModelCharacterController = new ThirdPersonCharacterController();
	// camera controller
	ICameraController* AnimModelCameraController = new ThirdPersonCameraController();
	Chroma::Scene::GetRenderCamera()->SetCustomCameraController(AnimModelCameraController);
	AnimModelCharacterController->SetCustomCameraController(AnimModelCameraController);
	// adding the component
	AnimModelEntity->AddComponent(AnimModelCharacterController);
	// ik
	AnimConstraintComponent* AnimModelIKComponent = new IKAnimConstraint();
	AnimModelEntity->AddComponent(AnimModelIKComponent);

	// ____________________________________________________


	// ____________________________________________________

	// IK TEST MODEL
	// ____________________________________________________
	IEntity* IKTestEntity = new Entity;
	IKTestEntity->SetName("IKTestEntity");
	Chroma::Scene::AddEntity(IKTestEntity);
	IKTestEntity->SetScale(glm::vec3(0.5f));
	//IKTestEntity->SetTranslation(glm::vec3(5.0, 0.0, 0.0));
	// mesh
	MeshComponent* IKTestMeshComponent = new SkinnedMesh("resources/animation/test_tentacle/test_tentacle_iktest.fbx");
	IKTestMeshComponent->SetShader(PBRShader);
	IKTestMeshComponent->AddTexture(gridAlbedo);
	IKTestMeshComponent->AddTexture(flatNormal);
	IKTestMeshComponent->m_UVMultiply = glm::vec2(8.0f);
	IKTestEntity->AddComponent(IKTestMeshComponent);
	// rigid
	PhysicsComponent* IKTestRigidComponent = new PhysicsComponent();
	IKTestRigidComponent->SetColliderShape(ColliderShape::Capsule);
	IKTestRigidComponent->SetCollisionState(ColliderState::Kinematic);
	IKTestEntity->AddComponent(IKTestRigidComponent);
	// animation
	AnimationComponent* IKTestAnimationComponent = new AnimationComponent();
	Animator IKTestAnimator;
	IKTestAnimator.BindSkeleton(IKTestMeshComponent);
	IKTestAnimator.LoadAnimations("resources/animation/test_tentacle/test_tentacle.fbx");
	IKTestAnimator.CompressAnimations();
	IKTestAnimationComponent->SetAnimator(IKTestAnimator);
	IKTestEntity->AddComponent(IKTestAnimationComponent);
	// ik
	AnimConstraintComponent* IKTestIKComponent = new IKAnimConstraint();
	IKTestEntity->AddComponent(IKTestIKComponent);
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
	// ground
	IEntity* TerrainEntity = new Entity;
	Chroma::Scene::AddEntity(TerrainEntity);

	MeshComponent* TerrainMeshComponent = new Model("resources/assets/level/ground.fbx");
	TerrainMeshComponent->SetShader(PBRShader);
	TerrainMeshComponent->AddTexture(woodBoardsAlbedo);
	TerrainMeshComponent->AddTexture(woodBoardsNormal);
	TerrainMeshComponent->AddTexture(woodBoardsMetRoughAO);
	TerrainMeshComponent->m_UVMultiply = glm::vec2(8.0f);
	TerrainEntity->AddComponent(TerrainMeshComponent);

	// rigid
	PhysicsComponent* TerrainPhysicsComponent = new PhysicsComponent();
	TerrainPhysicsComponent->SetColliderShape(ColliderShape::Convex);
	TerrainPhysicsComponent->SetCollisionState(ColliderState::Static);
	TerrainEntity->AddComponent(TerrainPhysicsComponent);

	// 10 degrees
	IEntity* TerrainEntity2 = new Entity;
	Chroma::Scene::AddEntity(TerrainEntity2);

	MeshComponent* TerrainMesh2Component = new Model("resources/assets/level/10degrees.fbx");
	TerrainMesh2Component->SetShader(PBRShader);
	TerrainMesh2Component->AddTexture(woodBoardsAlbedo);
	TerrainMesh2Component->AddTexture(woodBoardsNormal);
	TerrainMesh2Component->AddTexture(woodBoardsMetRoughAO);
	TerrainMesh2Component->m_UVMultiply = glm::vec2(8.0f);
	TerrainEntity2->AddComponent(TerrainMesh2Component);

	// rigid
	PhysicsComponent* TerrainPhysicsComponent2 = new PhysicsComponent();
	TerrainPhysicsComponent2->SetColliderShape(ColliderShape::Convex);
	TerrainPhysicsComponent2->SetCollisionState(ColliderState::Static);
	TerrainEntity2->AddComponent(TerrainPhysicsComponent2);


	// 20 degrees
	IEntity* TerrainEntity3 = new Entity;
	Chroma::Scene::AddEntity(TerrainEntity3);

	MeshComponent* TerrainMesh3Component = new Model("resources/assets/level/20degrees.fbx");
	TerrainMesh3Component->SetShader(PBRShader);
	TerrainMesh3Component->AddTexture(woodBoardsAlbedo);
	TerrainMesh3Component->AddTexture(woodBoardsNormal);
	TerrainMesh3Component->AddTexture(woodBoardsMetRoughAO);
	TerrainMesh3Component->m_UVMultiply = glm::vec2(8.0f);
	TerrainEntity3->AddComponent(TerrainMesh3Component);

	// rigid
	PhysicsComponent* TerrainPhysicsComponent3 = new PhysicsComponent();
	TerrainPhysicsComponent3->SetColliderShape(ColliderShape::Convex);
	TerrainPhysicsComponent3->SetCollisionState(ColliderState::Static);
	TerrainEntity3->AddComponent(TerrainPhysicsComponent3);

	// 35 degrees
	IEntity* TerrainEntity4 = new Entity;
	Chroma::Scene::AddEntity(TerrainEntity4);

	MeshComponent* TerrainMesh4Component = new Model("resources/assets/level/35degrees.fbx");
	TerrainMesh4Component->SetShader(PBRShader);
	TerrainMesh4Component->AddTexture(woodBoardsAlbedo);
	TerrainMesh4Component->AddTexture(woodBoardsNormal);
	TerrainMesh4Component->AddTexture(woodBoardsMetRoughAO);
	TerrainMesh4Component->m_UVMultiply = glm::vec2(8.0f);
	TerrainEntity4->AddComponent(TerrainMesh4Component);

	// rigid
	PhysicsComponent* TerrainPhysicsComponent4 = new PhysicsComponent();
	TerrainPhysicsComponent4->SetColliderShape(ColliderShape::Convex);
	TerrainPhysicsComponent4->SetCollisionState(ColliderState::Static);
	TerrainEntity4->AddComponent(TerrainPhysicsComponent4);

	

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
		SphereMeshComponent->SetShader(PBRShader);
		SphereEntity->SetTranslation(spherePositions[i]);
		SphereEntity->AddComponent(SphereMeshComponent);
		SphereEntity->AddComponent(SphereRigidComponent);
	}


	Chroma::Scene::PostSceneBuild();

	// RENDER LOOP
	// -----------
	while (Chroma::Screen::IsRunning())
	{
		// IK debug
		IKTestEntity->SetTranslation(glm::vec3(glm::cos(GAMETIME) * 5.0, glm::sin(GAMETIME) * 5.0, 0.0));
		static_cast<IKAnimConstraint*>(IKTestIKComponent)->SetEffectorWorldPos("tentacleTestIK", glm::vec3(glm::sin(GAMETIME) * 20.0, glm::cos(GAMETIME * 1.0) * 20.0, glm::sin(GAMETIME) * 20.0));

		if (Chroma::Input::IsPressed(Chroma::Input::J))
		{
			Chroma::Render::GetDebugBuffer()->ToggleDrawSkeletons();
		}

		if (Chroma::Input::IsPressed(Chroma::Input::C))
		{
			Chroma::Render::GetDebugBuffer()->ToggleDrawSkeletonConstraints();
		}

		// Physics debug
		if (Chroma::Input::IsPressed(Chroma::Input::P))
		{
			Chroma::Physics::ToggleDrawDebug();
		}
		

		// GAME TICK
		Chroma::Engine::Tick();

	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	Chroma::Screen::Close();
	return 0;
}