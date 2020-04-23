#include "Editor.h"

#include <resources/ResourceManager.h>


#include <entity/Entity.h>
#include <terrain/Terrain.h>
#include <component/AnimationComponent.h>
#include <model/SkinnedMesh.h>
#include <model/Model.h>
#include <bipedal/BipedalAnimationStateMachine.h>
#include <component/animation/IKAnimConstraint.h>
#include <component/CharacterPhysicsComponent.h>

// game
#include <thirdperson/ThirdPersonCharacterController.h>
#include <thirdperson/ThirdPersonCameraController.h>





namespace Chroma
{
	void Editor::Init()
	{
		Chroma::EditorUI::Init();
		CHROMA_INFO("CHROMA EDITOR :: Editor Initialized.");
	}

	void Editor::PopulateTestScene()
	{
		// LIGHTS
		// ____________________________________________________

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
		UnlitShader.SetUniform("color", glm::vec3(1, 1, 0));
		Shader SemiTransparentShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRShader("resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");

		// Lookdev Sphere
		Texture lookdevAlbedo("resources/textures/pbr/lookdev_pbr/albedo.jpg");
		lookdevAlbedo.m_Type = Texture::ALBEDO;
		Texture lookdevNormal("resources/textures/pbr/lookdev_pbr/normal.jpg");
		lookdevNormal.m_Type = Texture::NORMAL;
		Texture lookdevMetRoughAO("resources/textures/pbr/lookdev_pbr/MetRoughAO.jpg");
		lookdevMetRoughAO.m_Type = Texture::METROUGHAO;
		// ____________________________________________________
		// TEXTURES
		// ____________________________________________________
		// Generic
		Texture blackAlbedo("resources/textures/colors/black.jpg");
		Texture greyAlbedo("resources/textures/colors/grey.jpg");
		Texture whiteAlbedo("resources/textures/colors/white.jpg");
		Texture gridAlbedo("resources/animation/textures/grid.jpg");
		Texture flatNormal("resources/textures/test/flat_normal.jpg");
		flatNormal.m_Type = Texture::NORMAL;
		Texture alphaTestAlbedo("resources/textures/test/grass.png");

		// Animated Model
		Texture walkingAlbedo("resources/animation/vampire_textures/albedo.jpg");
		walkingAlbedo.m_Type = Texture::ALBEDO;
		Texture walkingNormal("resources/animation/vampire_textures/normal.jpg");
		walkingNormal.m_Type = Texture::NORMAL;
		Texture walkingMetRoughAO("resources/animation/vampire_textures/MetRoughAO.jpg");
		walkingMetRoughAO.m_Type = Texture::METROUGHAO;

		// Floor Panels
		Texture woodBoardsAlbedo("resources/textures/pbr/hardwood_pbr/albedo.jpg");
		woodBoardsAlbedo.m_Type = Texture::ALBEDO;
		Texture woodBoardsNormal("resources/textures/pbr/hardwood_pbr/normal.jpg");
		woodBoardsNormal.m_Type = Texture::NORMAL;
		Texture woodBoardsMetRoughAO("resources/textures/pbr/hardwood_pbr/MetRoughAO.jpg");
		woodBoardsMetRoughAO.m_Type = Texture::METROUGHAO;
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
		IKTestMeshComponent->GetMaterial().SetUVMultiply(8.0f);
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
		TerrainMeshComponent->GetMaterial().SetUVMultiply(8.0);
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
		TerrainMesh2Component->GetMaterial().SetUVMultiply(8.0);
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
		TerrainMesh3Component->GetMaterial().SetUVMultiply(8.0);
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
		TerrainMesh4Component->GetMaterial().SetUVMultiply(8.0);
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


		for (int i = 0; i < 5; i++)
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

	}

	void Editor::PopulateTestScene2()
	{
		// POPULATE SCENE CONTENTS
	// ------------------------------------------------------------------------------------------
	// LIGHTS
	// ____________________________________________________

		// POINT LIGHTS
		std::vector<Light*> Lights;

		// point light positions
		glm::vec3 pointLightPositions[] = {
			glm::vec3(3.5f,  1.2f,  2.0f),
			glm::vec3(0.5f,  0.2f,  -2.0f),
			glm::vec3(-3.5f,  1.2f,  4.0f),
		};
		// point lights
		for (glm::vec3 pos : pointLightPositions)
		{
			Light* pointLight = new Light(pos, Light::POINT);
			pointLight->setIntensity(0.51f);
			pointLight->m_Quadratic *= 4.0f;
			pointLight->m_Linear *= 2.0f;
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Light::SUNLIGHT, glm::vec3(-10.0, -1.0, -0.1), 2.0f);
		Sun->setDiffuse(glm::vec3(1.0));
		Sun->setIntensity(3.0);
		Lights.push_back(Sun);
		Chroma::Scene::SetLights(Lights);

		// ____________________________________________________
		// ENTITIES
		// ____________________________________________________

		IEntity* HumanEntity = new Entity;
		HumanEntity->SetName("Human Model");
		Chroma::Scene::AddEntity(HumanEntity);
		HumanEntity->SetScale(glm::vec3(40.0f));
		HumanEntity->SetTranslation(glm::vec3(-6.8, 0, 0));


		IEntity* LookDevEntity = new Entity;
		LookDevEntity->SetName("LookDev Sphere");
		Chroma::Scene::AddEntity(LookDevEntity);
		LookDevEntity->SetScale(glm::vec3(0.7f));
		LookDevEntity->SetTranslation(glm::vec3(6.2, 7.1, 0));

		// ____________________________________________________
		// SHADERS
		// ____________________________________________________

		Shader SemiTransparentShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRSkinShaderExperimental("resources/shaders/fragSSSS.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRSkinShader("resources/shaders/fragSSSS_backup.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRShader("resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");

		// ____________________________________________________
		// TEXTURES
		// ____________________________________________________

		//// Default
		// Generic
		Texture greyAlbedo("resources/textures/colors/grey.jpg");
		Texture gridAlbedo("resources/animation/textures/grid.jpg");
		Texture flatNormal("resources/textures/test/flat_normal.jpg");


		// Head
		Texture headAlbedo = Chroma::ResourceManager::LoadTexture("resources/human/textures/head/head_albedo.jpg");
		headAlbedo.m_Type = Texture::ALBEDO;
		Texture headNormal = Chroma::ResourceManager::LoadTexture("resources/human/textures/head/head_normal.jpg");
		headNormal.m_Type = Texture::NORMAL;
		Texture headMetRoughAO = Chroma::ResourceManager::LoadTexture("resources/human/textures/head/head_metroughao.jpg");
		headMetRoughAO.m_Type = Texture::METROUGHAO;
		Texture headTranslucency = Chroma::ResourceManager::LoadTexture("resources/human/textures/head/head_translucency.jpg");
		headTranslucency.m_Type = Texture::TRANSLUCENCY;

		// Lookdev Sphere
		//Texture lookDevAlbedo = Chroma::ResourceManager::LoadTexture("resources/textures/pbr/lookdev_pbr/albedo.jpg");
		//lookDevAlbedo.m_Type = Texture::ALBEDO;
		//Texture lookDevNormal = Chroma::ResourceManager::LoadTexture("resources/textures/pbr/lookdev_pbr/normal.jpg");
		//lookDevNormal.m_Type = Texture::NORMAL;
		//Texture lookDevMetRoughAO = Chroma::ResourceManager::LoadTexture("resources/textures/pbr/lookdev_pbr/MetRoughAO.jpg");
		//lookDevMetRoughAO.m_Type = Texture::METROUGHAO;

		// ____________________________________________________
		// MODELS
		// ____________________________________________________

		// HEAD		
		MeshComponent* HeadMeshComponent = new Model("resources/human/Head/Head.fbx");
		Material HeadMeshMaterial;
		HeadMeshMaterial.SetShader(PBRShader);
		HeadMeshMaterial.AddTexture(headAlbedo);
		HeadMeshMaterial.AddTexture(headNormal);
		HeadMeshMaterial.AddTexture(headMetRoughAO);
		HeadMeshMaterial.AddTexture(headTranslucency);
		HeadMeshMaterial.AddUniform("TestUniform", 1);
		HeadMeshMaterial.AddUniform("UVMultiply", glm::vec2(3.0));
		HeadMeshMaterial.AddUniform("Color", glm::vec3(0.5));
		HeadMeshMaterial.AddUniform("ColorAlpha", glm::vec4(0.5));
		HeadMeshComponent->SetMaterial(HeadMeshMaterial);
		HumanEntity->AddComponent(HeadMeshComponent);

		// Eyelashes 
		MeshComponent* EyelashesMeshComponent = new Model("resources/human/Head/Eyelashes.fbx");
		HumanEntity->AddComponent(EyelashesMeshComponent);

		// Eyebrows
		MeshComponent* EyebrowsMeshComponent = new Model("resources/human/Head/Eyebrows.fbx");
		HumanEntity->AddComponent(EyebrowsMeshComponent);

		// Head Physics
		PhysicsComponent* HeadPhysicsComponent = new PhysicsComponent();
		HeadPhysicsComponent->SetColliderShape(Box);
		HeadPhysicsComponent->SetCollisionState(Kinematic);
		HumanEntity->AddComponent(HeadPhysicsComponent);

		// LOOKDEVSPHERE
		//MeshComponent* lookDevMeshComponent = new Model("resources/lookdev/sphere.obj");
		//lookDevMeshComponent->SetShader(PBRShader);
		//lookDevMeshComponent->AddTexture(lookDevAlbedo);
		//lookDevMeshComponent->AddTexture(lookDevNormal);
		//lookDevMeshComponent->AddTexture(flatNormal);
		//LookDevEntity->AddComponent(lookDevMeshComponent);

		//// LookDev Physics
		//PhysicsComponent* LookDevPhysicsComponent = new PhysicsComponent();
		//LookDevPhysicsComponent->SetColliderShape(Box);
		//LookDevPhysicsComponent->SetCollisionState(Kinematic);
		//LookDevEntity->AddComponent(LookDevPhysicsComponent);


	}

	void Editor::PopulateTestScene3()
	{
		// POPULATE SCENE CONTENTS
		// ____________________________________________________
		// LIGHTS
		// ____________________________________________________

		// POINT LIGHTS
		std::vector<Light*> Lights;

		// point light positions
		glm::vec3 pointLightPositions[] = {
			glm::vec3(3.5f,  1.2f,  2.0f),
			glm::vec3(0.5f,  0.2f,  -2.0f),
			glm::vec3(-3.5f,  1.2f,  4.0f),
		};
		// point lights
		for (glm::vec3 pos : pointLightPositions)
		{
			Light* pointLight = new Light(pos, Light::POINT);
			pointLight->setIntensity(0.51f);
			pointLight->m_Quadratic *= 4.0f;
			pointLight->m_Linear *= 2.0f;
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Light::SUNLIGHT, glm::vec3(-10.0, -1.0, -0.1), 2.0f);
		Sun->setDiffuse(glm::vec3(1.0));
		Sun->setIntensity(3.0);
		Lights.push_back(Sun);
		Chroma::Scene::SetLights(Lights);


		// ____________________________________________________
		// SHADERS
		// ____________________________________________________

		Shader SemiTransparentShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRSkinShaderExperimental("resources/shaders/fragSSSS.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRSkinShader("resources/shaders/fragSSSS_backup.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRShader("resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");

		// ____________________________________________________
		// TEXTURES
		// ____________________________________________________

		// Default
		// Generic
		Texture greyAlbedo("resources/textures/colors/grey.jpg");
		greyAlbedo.m_Type = Texture::ALBEDO;
		Texture gridAlbedo("resources/animation/textures/grid.jpg");
		gridAlbedo.m_Type = Texture::ALBEDO;
		Texture flatNormal("resources/textures/test/flat_normal.jpg");
		flatNormal.m_Type = Texture::NORMAL;

		// Lookdev Sphere
		Texture lookDevAlbedo = Chroma::ResourceManager::LoadTexture("resources/textures/pbr/lookdev_pbr/albedo.jpg");
		lookDevAlbedo.m_Type = Texture::ALBEDO;
		Texture lookDevNormal = Chroma::ResourceManager::LoadTexture("resources/textures/pbr/lookdev_pbr/normal.jpg");
		lookDevNormal.m_Type = Texture::NORMAL;
		Texture lookDevMetRoughAO = Chroma::ResourceManager::LoadTexture("resources/textures/pbr/lookdev_pbr/MetRoughAO.jpg");
		lookDevMetRoughAO.m_Type = Texture::METROUGHAO;

		// ____________________________________________________
		// Materials
		// ____________________________________________________

		Material testMat;
		testMat.SetShader(PBRShader);
		testMat.AddTexture(lookDevAlbedo);
		testMat.AddTexture(lookDevNormal);
		testMat.AddTexture(lookDevMetRoughAO);

		Material testMat2;
		Material testMat3;
		Material testMat4;
		Material testMat5;

		std::vector<Material> materialList{ testMat, testMat2 , testMat3, testMat4, testMat5 };

		float spacing{ 3.0 };

		for (unsigned int i = 0; i < materialList.size(); i++)
		{
			IEntity* lookDevEntity = new Entity;
			lookDevEntity->SetName("LookDev Sphere : " + i);
			Chroma::Scene::AddEntity(lookDevEntity);
			lookDevEntity->SetScale(glm::vec3(0.2f));
			lookDevEntity->SetTranslation(glm::vec3((float)i * spacing, 2.1, 0));
			// mesh component
			MeshComponent* lookDevMeshComponent = new StaticMesh("resources/lookdev/sphere.obj");
			lookDevMeshComponent->SetMaterial(testMat);
			lookDevEntity->AddComponent(lookDevMeshComponent);

			// LookDev Physics
			PhysicsComponent* LookDevPhysicsComponent = new PhysicsComponent();
			LookDevPhysicsComponent->SetColliderShape(Box);
			LookDevPhysicsComponent->SetCollisionState(Kinematic);
			lookDevEntity->AddComponent(LookDevPhysicsComponent);
		}

	}


	void Editor::CornellBox()
	{
		// POPULATE SCENE CONTENTS
// ____________________________________________________
// LIGHTS
// ____________________________________________________

// POINT LIGHTS
		std::vector<Light*> Lights;

		// point light positions
		glm::vec3 pointLightPositions[] = {
			glm::vec3(3.5f,  1.2f,  2.0f),
			glm::vec3(0.5f,  0.2f,  -2.0f),
			glm::vec3(-3.5f,  1.2f,  4.0f),
		};
		// point lights
		for (glm::vec3 pos : pointLightPositions)
		{
			Light* pointLight = new Light(pos, Light::POINT);
			pointLight->setIntensity(0.51f);
			pointLight->m_Quadratic *= 4.0f;
			pointLight->m_Linear *= 2.0f;
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Light::SUNLIGHT, glm::vec3(-10.0, -1.0, -0.1), 2.0f);
		Sun->setDiffuse(glm::vec3(1.0));
		Sun->setIntensity(3.0);
		Lights.push_back(Sun);
		Chroma::Scene::SetLights(Lights);


		// ____________________________________________________
		// SHADERS
		// ____________________________________________________

		Shader SemiTransparentShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRSkinShaderExperimental("resources/shaders/fragSSSS.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRSkinShader("resources/shaders/fragSSSS_backup.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRShader("resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");

		// ____________________________________________________
		// TEXTURES
		// ____________________________________________________

		// Default
		// Generic
		Texture greyAlbedo("resources/textures/colors/grey.jpg");
		greyAlbedo.m_Type = Texture::ALBEDO;
		Texture gridAlbedo("resources/animation/textures/grid.jpg");
		gridAlbedo.m_Type = Texture::ALBEDO;
		Texture flatNormal("resources/textures/test/flat_normal.jpg");
		flatNormal.m_Type = Texture::NORMAL;

		// Lookdev Sphere
		Texture lookDevAlbedo = Chroma::ResourceManager::LoadTexture("resources/textures/pbr/lookdev_pbr/albedo.jpg");
		lookDevAlbedo.m_Type = Texture::ALBEDO;
		Texture lookDevNormal = Chroma::ResourceManager::LoadTexture("resources/textures/pbr/lookdev_pbr/normal.jpg");
		lookDevNormal.m_Type = Texture::NORMAL;
		Texture lookDevMetRoughAO = Chroma::ResourceManager::LoadTexture("resources/textures/pbr/lookdev_pbr/MetRoughAO.jpg");
		lookDevMetRoughAO.m_Type = Texture::METROUGHAO;

		// ____________________________________________________
		// Materials
		// ____________________________________________________

		Material wallMat;
		wallMat.SetShader(PBRShader);
		wallMat.AddTexture(lookDevAlbedo);
		wallMat.AddTexture(flatNormal);
		wallMat.AddTexture(lookDevMetRoughAO);

		Material ballMatteMat;
		ballMatteMat.AddTexture(greyAlbedo);
		ballMatteMat.AddTexture(flatNormal);


		// ____________________________________________________
		// Entities
		// ____________________________________________________
		IEntity* CornellBoxEntity = new Entity;
		CornellBoxEntity->SetName("Cornel Box");
		Chroma::Scene::AddEntity(CornellBoxEntity);

		// ____________________________________________________
		// Components
		// ____________________________________________________

		// Mesh component
		// walls
		MeshComponent* wallBackMeshComponent = new StaticMesh("resources/lookdev/CornellBox/sphereBackWall.fbx");
		wallBackMeshComponent->SetMaterial(wallMat);
		CornellBoxEntity->AddComponent(wallBackMeshComponent);
		MeshComponent* wallLeftMeshComponent = new StaticMesh("resources/lookdev/CornellBox/sphereLeftWall.fbx");
		wallLeftMeshComponent->SetMaterial(wallMat);
		CornellBoxEntity->AddComponent(wallLeftMeshComponent);
		MeshComponent* wallRightMeshComponent = new StaticMesh("resources/lookdev/CornellBox/sphereRightWall.fbx");
		wallRightMeshComponent->SetMaterial(wallMat);
		CornellBoxEntity->AddComponent(wallRightMeshComponent);
		MeshComponent* wallCeilingMeshComponent = new StaticMesh("resources/lookdev/CornellBox/sphereCeiling.fbx");
		wallCeilingMeshComponent->SetMaterial(wallMat);
		CornellBoxEntity->AddComponent(wallCeilingMeshComponent);
		MeshComponent* wallFloorMeshComponent = new StaticMesh("resources/lookdev/CornellBox/sphereFloor.fbx");
		wallFloorMeshComponent->SetMaterial(wallMat);
		CornellBoxEntity->AddComponent(wallFloorMeshComponent);
		// balls
		MeshComponent* leftBallMeshComponent = new StaticMesh("resources/lookdev/CornellBox/sphereLeft.fbx");
		leftBallMeshComponent->SetMaterial(ballMatteMat);
		CornellBoxEntity->AddComponent(leftBallMeshComponent);
		MeshComponent* leftRightMeshComponent = new StaticMesh("resources/lookdev/CornellBox/sphereRight.fbx");
		leftRightMeshComponent->SetMaterial(ballMatteMat);
		CornellBoxEntity->AddComponent(leftRightMeshComponent);



		// Physics
		PhysicsComponent* CornellBoxPhysicsComponent = new PhysicsComponent();
		CornellBoxPhysicsComponent->SetColliderShape(Box);
		CornellBoxPhysicsComponent->SetCollisionState(Kinematic);
		CornellBoxEntity->AddComponent(CornellBoxPhysicsComponent);
	}

	void Editor::Sponza()
	{	
		// POINT LIGHTS
		std::vector<Light*> Lights;

		// point light positions
		glm::vec3 pointLightPositions[] = {
			glm::vec3(3.5f,  1.2f,  2.0f),
			glm::vec3(0.5f,  0.2f,  -2.0f),
			glm::vec3(-3.5f,  1.2f,  4.0f),
		};
		// point lights
		for (glm::vec3 pos : pointLightPositions)
		{
			Light* pointLight = new Light(pos, Light::POINT);
			pointLight->setIntensity(0.51f);
			pointLight->m_Quadratic *= 4.0f;
			pointLight->m_Linear *= 2.0f;
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Light::SUNLIGHT, glm::vec3(-10.0, -1.0, -0.1), 2.0f);
		Sun->setDiffuse(glm::vec3(1.0));
		Sun->setIntensity(3.0);
		Lights.push_back(Sun);
		Chroma::Scene::SetLights(Lights);


		// ____________________________________________________
		// SHADERS
		// ____________________________________________________

		Shader SemiTransparentShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRShader("resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");

		// ____________________________________________________
		// TEXTURES
		// ____________________________________________________

		// Default
		// Generic
		Texture greyAlbedo("resources/textures/colors/grey.jpg");
		greyAlbedo.m_Type = Texture::ALBEDO;
		Texture gridAlbedo("resources/animation/textures/grid.jpg");
		gridAlbedo.m_Type = Texture::ALBEDO;
		Texture flatNormal("resources/textures/test/flat_normal.jpg");
		flatNormal.m_Type = Texture::NORMAL;

		Texture materialRedAlbedo("resources/lookdev/Sponza/textures/sponza_fabric_diff.png");
		materialRedAlbedo.m_Type = Texture::ALBEDO;
		Texture materialNormal("resources/lookdev/Sponza/textures/sponza_fabric_diff_NRM.jpg");
		materialNormal.m_Type = Texture::NORMAL;
		
	

		// ____________________________________________________
		// Materials
		// ____________________________________________________

		Material basicMat;
		basicMat.SetShader(PBRShader);
		basicMat.AddTexture(greyAlbedo);
		basicMat.AddTexture(flatNormal);

		Material ClothMaterial;
		ClothMaterial.SetShader(PBRShader);
		ClothMaterial.AddTexture(materialRedAlbedo);
		ClothMaterial.AddTexture(materialNormal);


		// ____________________________________________________
		// Entities
		// ____________________________________________________
		IEntity* SponzaEntity = new Entity;
		SponzaEntity->SetName("Sponza");
		SponzaEntity->SetScale(glm::vec3(0.01));
		Chroma::Scene::AddEntity(SponzaEntity);

		// ____________________________________________________
		// Components
		// ____________________________________________________

		// Mesh component
		MeshComponent* SponzaMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza.obj");
		SponzaMeshComponent->SetMaterial(basicMat);
		SponzaEntity->AddComponent(SponzaMeshComponent);

		MeshComponent* SponzaMaterialMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_material.obj");
		SponzaMaterialMeshComponent->SetMaterial(ClothMaterial);
		SponzaEntity->AddComponent(SponzaMaterialMeshComponent);
	}

	void Editor::Dragon()
	{
		// POINT LIGHTS
		std::vector<Light*> Lights;

		// point light positions
		glm::vec3 pointLightPositions[] = {
			glm::vec3(3.5f,  1.2f,  2.0f),
			glm::vec3(0.5f,  0.2f,  -2.0f),
			glm::vec3(-3.5f,  1.2f,  4.0f),
		};
		// point lights
		for (glm::vec3 pos : pointLightPositions)
		{
			Light* pointLight = new Light(pos, Light::POINT);
			pointLight->setIntensity(0.51f);
			pointLight->m_Quadratic *= 4.0f;
			pointLight->m_Linear *= 2.0f;
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Light::SUNLIGHT, glm::vec3(-10.0, -1.0, -0.1), 2.0f);
		Sun->setDiffuse(glm::vec3(1.0));
		Sun->setIntensity(3.0);
		Lights.push_back(Sun);
		Chroma::Scene::SetLights(Lights);


		// ____________________________________________________
		// SHADERS
		// ____________________________________________________

		Shader SemiTransparentShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRShader("resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");

		// ____________________________________________________
		// TEXTURES
		// ____________________________________________________

		// Default
		// Generic
		Texture greyAlbedo("resources/textures/colors/grey.jpg");
		greyAlbedo.m_Type = Texture::ALBEDO;
		Texture gridAlbedo("resources/animation/textures/grid.jpg");
		gridAlbedo.m_Type = Texture::ALBEDO;
		Texture flatNormal("resources/textures/test/flat_normal.jpg");
		flatNormal.m_Type = Texture::NORMAL;

		// ____________________________________________________
		// Materials
		// ____________________________________________________

		Material basicMat;
		basicMat.SetShader(PBRShader);
		basicMat.AddTexture(greyAlbedo);
		basicMat.AddTexture(flatNormal);


		// ____________________________________________________
		// Entities
		// ____________________________________________________
		IEntity* DragonEntity = new Entity;
		DragonEntity->SetName("Dragon");
		Chroma::Scene::AddEntity(DragonEntity);

		// ____________________________________________________
		// Components
		// ____________________________________________________

		// Mesh component
		MeshComponent* DragonMeshComponent = new StaticMesh("resources/lookdev/Dragon/dragon.fbx");
		DragonMeshComponent->SetMaterial(basicMat);
		DragonEntity->AddComponent(DragonMeshComponent);
	}

	void Editor::Buddha()
	{
		// POINT LIGHTS
		std::vector<Light*> Lights;

		// point light positions
		glm::vec3 pointLightPositions[] = {
			glm::vec3(3.5f,  1.2f,  2.0f),
			glm::vec3(0.5f,  0.2f,  -2.0f),
			glm::vec3(-3.5f,  1.2f,  4.0f),
		};
		// point lights
		for (glm::vec3 pos : pointLightPositions)
		{
			Light* pointLight = new Light(pos, Light::POINT);
			pointLight->setIntensity(0.51f);
			pointLight->m_Quadratic *= 4.0f;
			pointLight->m_Linear *= 2.0f;
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Light::SUNLIGHT, glm::vec3(-10.0, -1.0, -0.1), 2.0f);
		Sun->setDiffuse(glm::vec3(1.0));
		Sun->setIntensity(3.0);
		Lights.push_back(Sun);
		Chroma::Scene::SetLights(Lights);



		// ____________________________________________________
		// SHADERS
		// ____________________________________________________

		Shader SemiTransparentShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRShader("resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");

		// ____________________________________________________
		// TEXTURES
		// ____________________________________________________

		// Default
		// Generic
		Texture greyAlbedo("resources/textures/colors/grey.jpg");
		greyAlbedo.m_Type = Texture::ALBEDO;
		Texture gridAlbedo("resources/animation/textures/grid.jpg");
		gridAlbedo.m_Type = Texture::ALBEDO;
		Texture flatNormal("resources/textures/test/flat_normal.jpg");
		flatNormal.m_Type = Texture::NORMAL;
		Texture lookDevMetRoughAO = Chroma::ResourceManager::LoadTexture("resources/textures/pbr/lookdev_pbr/MetRoughAO.jpg");
		lookDevMetRoughAO.m_Type = Texture::METROUGHAO;

		// ____________________________________________________
		// Materials
		// ____________________________________________________

		Material basicMat;
		basicMat.SetShader(PBRShader);
		basicMat.AddTexture(greyAlbedo);
		basicMat.AddTexture(flatNormal);
		basicMat.AddTexture(lookDevMetRoughAO);



		// ____________________________________________________
		// Entities
		// ____________________________________________________
		IEntity* BuddhaEntity = new Entity;
		BuddhaEntity->SetName("BuddhaEntity");
		Chroma::Scene::AddEntity(BuddhaEntity);

		// ____________________________________________________
		// Components
		// ____________________________________________________

		// Mesh component
		MeshComponent* BuddhaMeshComponent = new StaticMesh("resources/lookdev/Buddha/buddha.fbx");
		BuddhaMeshComponent->SetMaterial(basicMat);
		BuddhaEntity->AddComponent(BuddhaMeshComponent);
	}

	void Editor::StanfordBunny()
	{
		// POINT LIGHTS
		std::vector<Light*> Lights;

		// point light positions
		glm::vec3 pointLightPositions[] = {
			glm::vec3(3.5f,  1.2f,  2.0f),
			glm::vec3(0.5f,  0.2f,  -2.0f),
			glm::vec3(-3.5f,  1.2f,  4.0f),
		};
		// point lights
		for (glm::vec3 pos : pointLightPositions)
		{
			Light* pointLight = new Light(pos, Light::POINT);
			pointLight->setIntensity(0.51f);
			pointLight->m_Quadratic *= 4.0f;
			pointLight->m_Linear *= 2.0f;
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Light::SUNLIGHT, glm::vec3(-10.0, -1.0, -0.1), 2.0f);
		Sun->setDiffuse(glm::vec3(1.0));
		Sun->setIntensity(3.0);
		Lights.push_back(Sun);
		Chroma::Scene::SetLights(Lights);


		// ____________________________________________________
		// SHADERS
		// ____________________________________________________

		Shader SemiTransparentShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRShader("resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");

		// ____________________________________________________
		// TEXTURES
		// ____________________________________________________

		// Default
		// Generic
		Texture greyAlbedo("resources/textures/colors/grey.jpg");
		greyAlbedo.m_Type = Texture::ALBEDO;
		Texture gridAlbedo("resources/animation/textures/grid.jpg");
		gridAlbedo.m_Type = Texture::ALBEDO;
		Texture flatNormal("resources/textures/test/flat_normal.jpg");
		flatNormal.m_Type = Texture::NORMAL;

		// ____________________________________________________
		// Materials
		// ____________________________________________________

		Material basicMat;
		basicMat.SetShader(PBRShader);
		basicMat.AddTexture(greyAlbedo);
		basicMat.AddTexture(flatNormal);



		// ____________________________________________________
		// Entities
		// ____________________________________________________
		IEntity* SponzaEntity = new Entity;
		SponzaEntity->SetName("Sponza");
		Chroma::Scene::AddEntity(SponzaEntity);

		// ____________________________________________________
		// Components
		// ____________________________________________________

		// Mesh component
		MeshComponent* SponzaMeshComponent = new StaticMesh("resources/lookdev/StanfordBunny/bunny.fbx");
		SponzaMeshComponent->SetMaterial(basicMat);
		SponzaEntity->AddComponent(SponzaMeshComponent);
	}


	void Editor::Tick()
	{
		// UI
		DrawUI();
	}

	void Editor::DrawUI()
	{
		Chroma::EditorUI::Draw();
	}

	
}