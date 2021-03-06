//#ifndef EDITOR
////common
//#include <common/PrecompiledHeader.h>
//
//// chroma
//#include <engine/engine.h>
//#include <entity/Entity.h>
//#include <terrain/Terrain.h>
//#include <component/AnimationComponent.h>
//#include <component/PhysicsComponent.h>
//#include <model/SkinnedMesh.h>
//#include <bipedal/BipedalAnimationStateMachine.h>
//#include <component/animation/IKAnimConstraint.h>
//
//// game
//#include <thirdperson/ThirdPersonCharacterController.h>
//#include <thirdperson/ThirdPersonCameraController.h>
//
//
//
//
//int main()
//{
//	// INIT CHROMA
//	Chroma::Engine::Init();
//
//	// Initialize Scene
//	Chroma::Scene::PreSceneBuild();
//
//	// SCENE CONTENTS
//	// ------------------------------------------------------------------------------------------
//	// LIGHTS
//	// ____________________________________________________
//	const float SUNLIGHT_SPIN_SPEED = .8f;
//	const float SUNLIGHT_DISTANCE = 15.0f;
//
//	// POINT LIGHTS
//	std::vector<Light*> Lights;
//
//	// point light positions
//	glm::vec3 pointLightPositions[] = {
//		glm::vec3(3.5f,  1.2f,  2.0f),
//		glm::vec3(0.5f,  0.2f,  -2.0f),
//		glm::vec3(-3.5f,  1.2f,  4.0f),
//	};
//	// dancing point lights
//	for (glm::vec3 pos : pointLightPositions)
//	{
//		Light* pointLight = new Light(pos, Chroma::Type::Light::kPointLight);
//		Lights.push_back(pointLight);
//	}
//
//	// SUNLIGHT
//	Light* Sun = new Light(Chroma::Type::Light::kSunlight, glm::vec3(0.2, -0.8, 0.3), 2.0f);
//	Sun->SetDiffuse(glm::vec3(1.0));
//	Sun->SetIntensity(3.0);
//	Lights.push_back(Sun);
//	Chroma::Scene::SetLights(Lights);
//
//	// BACKGROUND
//	//Chroma::Scene::GetIBL()->LoadIBL("resources/textures/ibl/grey_ibl/grey.hdr");
//
//	// ____________________________________________________
//	// SHADERS
//	// ____________________________________________________
//	Shader UnlitShader("resources/shaders/fragBasic.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
//	UnlitShader.Use();
//	UnlitShader.SetUniform("color", glm::vec3(1, 1, 0));
//	Shader SemiTransparentShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
//	Shader PBRShader("resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
//
//	// Lookdev Sphere
//	Texture lookdevAlbedo("resources/textures/pbr/lookdev_pbr/albedo.jpg");
//	lookdevAlbedo.SetType(Chroma::Type::Texture::kAlbedo);
//	Texture lookdevNormal("resources/textures/pbr/lookdev_pbr/normal.jpg");
//	lookdevNormal.SetType(Chroma::Type::Texture::kNormal);
//	Texture lookdevMetRoughAO("resources/textures/pbr/lookdev_pbr/MetRoughAO.jpg");
//	lookdevMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);
//	// ____________________________________________________
//	// TEXTURES
//	// ____________________________________________________
//	// Generic
//	Texture blackAlbedo("resources/textures/colors/black.jpg");
//	Texture greyAlbedo("resources/textures/colors/grey.jpg");
//	Texture whiteAlbedo("resources/textures/colors/white.jpg");
//	Texture gridAlbedo("resources/animation/textures/grid.jpg");
//	Texture flatNormal("resources/textures/test/flat_normal.jpg");
//	flatNormal.SetType(Chroma::Type::Texture::kNormal);
//	Texture alphaTestAlbedo("resources/textures/test/grass.png");
//
//	// Animated Model
//	Texture walkingAlbedo("resources/animation/vampire_textures/albedo.jpg");
//	walkingAlbedo.SetType(Chroma::Type::Texture::kAlbedo);
//	Texture walkingNormal("resources/animation/vampire_textures/normal.jpg");
//	walkingNormal.SetType(Chroma::Type::Texture::kNormal);
//	Texture walkingMetRoughAO("resources/animation/vampire_textures/MetRoughAO.jpg");
//	walkingMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);
//
//	// Floor Panels
//	Texture woodBoardsAlbedo("resources/textures/pbr/hardwood_pbr/albedo.jpg");
//	woodBoardsAlbedo.SetType(Chroma::Type::Texture::kAlbedo);
//	Texture woodBoardsNormal("resources/textures/pbr/hardwood_pbr/normal.jpg");
//	woodBoardsNormal.SetType(Chroma::Type::Texture::kNormal);
//	Texture woodBoardsMetRoughAO("resources/textures/pbr/hardwood_pbr/MetRoughAO.jpg");
//	woodBoardsMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);
//	// ____________________________________________________
//
//	// ANIMATED MODEL
//	// ____________________________________________________
//	IEntity* AnimModelEntity = new Entity;
//	AnimModelEntity->SetName("AnimationModel");
//	Chroma::Scene::AddEntity(AnimModelEntity);
//	AnimModelEntity->SetScale(glm::vec3(0.03f));
//	//AnimModelEntity->SetTranslation(glm::vec3(0.0, 5.0, 0.0));
//	// mesh
//	MeshComponent* AnimModelMeshComponent = new SkinnedMesh("resources/animation/vampire.fbx");
//	AnimModelMeshComponent->AddTexture(walkingAlbedo);
//	AnimModelMeshComponent->AddTexture(walkingNormal);
//	AnimModelMeshComponent->AddTexture(walkingMetRoughAO);
//	AnimModelMeshComponent->SetIsDoubleSided(true);
//	AnimModelEntity->AddComponent(AnimModelMeshComponent);
//	// rigid
//	PhysicsComponent* AnimModelRigidComponent = new PhysicsComponent();
//	AnimModelRigidComponent->SetColliderShape(ColliderShape::Capsule);
//	AnimModelRigidComponent->SetCollisionState(ColliderState::Kinematic);
//	AnimModelEntity->AddComponent(AnimModelRigidComponent);
//	// animation
//	AnimationComponent* AnimModelAnimationComponent = new AnimationComponent();
//	Animator AnimModelAnimator;
//	AnimModelAnimator.BindSkeleton(AnimModelMeshComponent);
//	AnimModelAnimator.LoadAnimations("resources/animation/locomotion/Walk.fbx");
//	AnimModelAnimator.LoadAnimations("resources/animation/locomotion/Idle.fbx");
//	AnimModelAnimator.LoadAnimations("resources/animation/locomotion/Run.fbx");
//	AnimModelAnimator.LoadAnimations("resources/animation/locomotion/Jump.fbx");
//	AnimModelAnimator.LoadAnimations("resources/animation/locomotion/Roll.fbx");
//	AnimModelAnimator.CompressAnimations();
//	//AnimationStateMachine* AnimModelAnimationStateMachine = new BipedalAnimationStateMachine;
//	//// animation state
//	//AnimModelAnimationComponent->SetAnimator(AnimModelAnimator);
//	//AnimModelAnimationComponent->SetAnimationStateMachine(AnimModelAnimationStateMachine);
//	//AnimModelEntity->AddComponent(AnimModelAnimationComponent);
//	////// character controller
//	//CharacterControllerComponent* AnimModelCharacterController = new ThirdPersonCharacterController();
//	//// camera controller
//	//ICameraController* AnimModelCameraController = new ThirdPersonCameraController();
//	//Chroma::Scene::GetRenderCamera()->SetCustomCameraController(AnimModelCameraController);
//	//AnimModelCharacterController->SetCustomCameraController(AnimModelCameraController);
//	//// adding the component
//	//AnimModelEntity->AddComponent(AnimModelCharacterController);
//	// ik
//	AnimConstraintComponent* AnimModelIKComponent = new IKAnimConstraint();
//	AnimModelEntity->AddComponent(AnimModelIKComponent);
//
//
//
//	// ____________________________________________________
//
//	// IK TEST MODEL
//	// ____________________________________________________
//	IEntity* IKTestEntity = new Entity;
//	IKTestEntity->SetName("IKTestEntity");
//	Chroma::Scene::AddEntity(IKTestEntity);
//	IKTestEntity->SetScale(glm::vec3(0.5f));
//	//IKTestEntity->SetTranslation(glm::vec3(5.0, 0.0, 0.0));
//	// mesh
//	MeshComponent* IKTestMeshComponent = new SkinnedMesh("resources/animation/test_tentacle/test_tentacle_iktest.fbx");
//	IKTestMeshComponent->SetShader(PBRShader);
//	IKTestMeshComponent->AddTexture(gridAlbedo);
//	IKTestMeshComponent->AddTexture(flatNormal);
//	IKTestMeshComponent->GetMaterial().SetUVMultiply(8.0f);
//	IKTestEntity->AddComponent(IKTestMeshComponent);
//	// rigid
//	PhysicsComponent* IKTestRigidComponent = new PhysicsComponent();
//	IKTestRigidComponent->SetColliderShape(ColliderShape::Capsule);
//	IKTestRigidComponent->SetCollisionState(ColliderState::Kinematic);
//	IKTestEntity->AddComponent(IKTestRigidComponent);
//	// animation
//	AnimationComponent* IKTestAnimationComponent = new AnimationComponent();
//	Animator IKTestAnimator;
//	IKTestAnimator.BindSkeleton(IKTestMeshComponent);
//	IKTestAnimator.LoadAnimations("resources/animation/test_tentacle/test_tentacle.fbx");
//	IKTestAnimator.CompressAnimations();
//	IKTestAnimationComponent->SetAnimator(IKTestAnimator);
//	IKTestEntity->AddComponent(IKTestAnimationComponent);
//	// ik
//	AnimConstraintComponent* IKTestIKComponent = new IKAnimConstraint();
//	IKTestEntity->AddComponent(IKTestIKComponent);
//	// ____________________________________________________
//
//
//	// TERRAIN
//	// ____________________________________________________
//	// ground
//	IEntity* TerrainEntity = new Entity;
//	Chroma::Scene::AddEntity(TerrainEntity);
//
//	MeshComponent* TerrainMeshComponent = new Model("resources/assets/level/ground.fbx");
//	TerrainMeshComponent->SetShader(PBRShader);
//	TerrainMeshComponent->AddTexture(woodBoardsAlbedo);
//	TerrainMeshComponent->AddTexture(woodBoardsNormal);
//	TerrainMeshComponent->AddTexture(woodBoardsMetRoughAO);
//	TerrainMeshComponent->GetMaterial().SetUVMultiply(8.0);
//	TerrainEntity->AddComponent(TerrainMeshComponent);
//
//	// rigid
//	PhysicsComponent* TerrainPhysicsComponent = new PhysicsComponent();
//	TerrainPhysicsComponent->SetColliderShape(ColliderShape::Convex);
//	TerrainPhysicsComponent->SetCollisionState(ColliderState::Static);
//	TerrainEntity->AddComponent(TerrainPhysicsComponent);
//
//	// 10 degrees
//	IEntity* TerrainEntity2 = new Entity;
//	Chroma::Scene::AddEntity(TerrainEntity2);
//
//	MeshComponent* TerrainMesh2Component = new Model("resources/assets/level/10degrees.fbx");
//	TerrainMesh2Component->SetShader(PBRShader);
//	TerrainMesh2Component->AddTexture(woodBoardsAlbedo);
//	TerrainMesh2Component->AddTexture(woodBoardsNormal);
//	TerrainMesh2Component->AddTexture(woodBoardsMetRoughAO);
//	TerrainMesh2Component->GetMaterial().SetUVMultiply(8.0);
//	TerrainEntity2->AddComponent(TerrainMesh2Component);
//
//	// rigid
//	PhysicsComponent* TerrainPhysicsComponent2 = new PhysicsComponent();
//	TerrainPhysicsComponent2->SetColliderShape(ColliderShape::Convex);
//	TerrainPhysicsComponent2->SetCollisionState(ColliderState::Static);
//	TerrainEntity2->AddComponent(TerrainPhysicsComponent2);
//
//
//	// 20 degrees
//	IEntity* TerrainEntity3 = new Entity;
//	Chroma::Scene::AddEntity(TerrainEntity3);
//
//	MeshComponent* TerrainMesh3Component = new Model("resources/assets/level/20degrees.fbx");
//	TerrainMesh3Component->SetShader(PBRShader);
//	TerrainMesh3Component->AddTexture(woodBoardsAlbedo);
//	TerrainMesh3Component->AddTexture(woodBoardsNormal);
//	TerrainMesh3Component->AddTexture(woodBoardsMetRoughAO);
//	TerrainMesh3Component->GetMaterial().SetUVMultiply(8.0);
//	TerrainEntity3->AddComponent(TerrainMesh3Component);
//
//	// rigid
//	PhysicsComponent* TerrainPhysicsComponent3 = new PhysicsComponent();
//	TerrainPhysicsComponent3->SetColliderShape(ColliderShape::Convex);
//	TerrainPhysicsComponent3->SetCollisionState(ColliderState::Static);
//	TerrainEntity3->AddComponent(TerrainPhysicsComponent3);
//
//	// 35 degrees
//	IEntity* TerrainEntity4 = new Entity;
//	Chroma::Scene::AddEntity(TerrainEntity4);
//
//	MeshComponent* TerrainMesh4Component = new Model("resources/assets/level/35degrees.fbx");
//	TerrainMesh4Component->SetShader(PBRShader);
//	TerrainMesh4Component->AddTexture(woodBoardsAlbedo);
//	TerrainMesh4Component->AddTexture(woodBoardsNormal);
//	TerrainMesh4Component->AddTexture(woodBoardsMetRoughAO);
//	TerrainMesh4Component->GetMaterial().SetUVMultiply(8.0);
//	TerrainEntity4->AddComponent(TerrainMesh4Component);
//
//	// rigid
//	PhysicsComponent* TerrainPhysicsComponent4 = new PhysicsComponent();
//	TerrainPhysicsComponent4->SetColliderShape(ColliderShape::Convex);
//	TerrainPhysicsComponent4->SetCollisionState(ColliderState::Static);
//	TerrainEntity4->AddComponent(TerrainPhysicsComponent4);
//
//	
//
//	// ____________________________________________________
//
//	// SPHERES
//	// Sphere Positions
//	glm::vec3 spherePositions[] = {
//		glm::vec3(0.f,  1.0f,  0.0f),
//		glm::vec3(2.5f,  1.0f,  0.0f),
//		glm::vec3(5.0f,  1.0f,  0.0f),
//		glm::vec3(-2.5f,  1.0f,  0.0f),
//		glm::vec3(-5.0f,  1.0f,  0.0f),
//	};
//
//	
//	for (int i =0; i < 5; i++ )
//	{
//		IEntity* SphereEntity = new Entity;
//		Chroma::Scene::AddEntity(SphereEntity);
//		SphereEntity->SetName("Sphere");
//		MeshComponent* SphereMeshComponent = new SpherePrimitive();
//		PhysicsComponent* SphereRigidComponent = new PhysicsComponent();
//		SphereRigidComponent->SetColliderShape(ColliderShape::Sphere);
//		SphereRigidComponent->SetCollisionState(ColliderState::Dynamic);
//		SphereRigidComponent->SetMass(1.0f);
//		SphereRigidComponent->SetFriction(3.0f);
//		SphereMeshComponent->AddTexture(greyAlbedo);
//		SphereEntity->SetTranslation(spherePositions[i]);
//		SphereEntity->AddComponent(SphereMeshComponent);
//		SphereEntity->AddComponent(SphereRigidComponent);
//	}
//
//
//	Chroma::Scene::PostSceneBuild();
//	// RENDER LOOP
//	// -----------
//	while (Chroma::Screen::IsRunning())
//	{
//		// IK debug
//		IKTestEntity->SetTranslation(glm::vec3(glm::cos(GAMETIME) * 2.0, (glm::sin(GAMETIME) * 2.0 + 10), 0.0));
//
//		//IKTestEntity->SetRotation(glm::angleAxis((float)(glm::radians(glm::cos(GAMETIME) * 180)), glm::abs(glm::vec3(glm::sin(GAMETIME*0.1),  glm::cos(GAMETIME * 0.1), glm::sin(GAMETIME * 0.1)))));
//		static_cast<IKAnimConstraint*>(IKTestIKComponent)->SetEffectorWorldPos("tentacleTestIK", glm::vec3(glm::sin(GAMETIME) * 5.0, glm::abs( glm::cos(GAMETIME * 1.0) * 8.0) + 18, glm::sin(GAMETIME) * 5.0));
//
//		if (Chroma::Input::IsPressed(Chroma::Input::J))
//		{
//			Chroma::Render::GetDebugBuffer()->ToggleDrawSkeletons();
//		}
//
//		if (Chroma::Input::IsPressed(Chroma::Input::C))
//		{
//			Chroma::Render::GetDebugBuffer()->ToggleDrawSkeletonConstraints();
//		}
//
//		// Physics debug
//		if (Chroma::Input::IsPressed(Chroma::Input::P))
//		{
//			Chroma::Physics::ToggleDrawDebug();
//		}
//		
//
//		// GAME TICK
//		Chroma::Engine::OnUpdate();
//
//	}
//
//	// glfw: terminate, clearing all previously allocated GLFW resources.
//	// ------------------------------------------------------------------
//	Chroma::Screen::Close();
//	return 0;
//}
//
//#endif