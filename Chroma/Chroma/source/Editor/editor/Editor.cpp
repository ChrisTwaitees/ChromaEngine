#include "Editor.h"

#include <resources/ResourceManager.h>


#include <entity/Entity.h>
#include <terrain/Terrain.h>
#include <component/AnimationComponent.h>
#include <model/SkinnedMesh.h>
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
			Light* pointLight = new Light(pos, Chroma::Type::Light::kPointLight);
			pointLight->SetIntensity(10.0f);
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Chroma::Type::Light::kSunlight, glm::vec3(0.2, -0.8, 0.3), 2.0f);
		Sun->SetDiffuse(glm::vec3(1.0));
		Sun->SetIntensity(3.0);
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
		Texture lookdevAlbedo("resources/textures/pbr/lookdev_pbr/albedo.jpg", Chroma::Type::Texture::kAlbedo);
		Texture lookdevNormal("resources/textures/pbr/lookdev_pbr/normal.jpg", Chroma::Type::Texture::kNormal);
		Texture lookdevMetRoughAO("resources/textures/pbr/lookdev_pbr/MetRoughAO.jpg", Chroma::Type::Texture::kMetRoughAO);
		// ____________________________________________________
		// TEXTURES
		// ____________________________________________________
		// Generic
		Texture blackAlbedo("resources/textures/colors/black.jpg");
		Texture greyAlbedo("resources/textures/colors/grey.jpg");
		Texture whiteAlbedo("resources/textures/colors/white.jpg");
		Texture gridAlbedo("resources/animation/textures/grid.jpg");
		Texture flatNormal("resources/textures/test/flat_normal.jpg", Chroma::Type::Texture::kNormal);

		// Animated Model
		Texture walkingAlbedo("resources/animation/vampire_textures/albedo.jpg", Chroma::Type::Texture::kAlbedo);
		Texture walkingNormal("resources/animation/vampire_textures/normal.jpg", Chroma::Type::Texture::kNormal);
		Texture walkingMetRoughAO("resources/animation/vampire_textures/MetRoughAO.jpg");
		walkingMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);

		// Floor Panels
		Texture woodBoardsAlbedo("resources/textures/pbr/hardwood_pbr/albedo.jpg", Chroma::Type::Texture::kAlbedo);
		Texture woodBoardsNormal("resources/textures/pbr/hardwood_pbr/normal.jpg", Chroma::Type::Texture::kNormal);
		Texture woodBoardsMetRoughAO("resources/textures/pbr/hardwood_pbr/MetRoughAO.jpg");
		woodBoardsMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);
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
		std::shared_ptr<ICameraController> AnimModelCameraController = std::make_shared<ThirdPersonCameraController>();
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

		MeshComponent* TerrainMeshComponent = new StaticMesh("resources/assets/level/ground.fbx");
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

		MeshComponent* TerrainMesh2Component = new StaticMesh("resources/assets/level/10degrees.fbx");
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

		MeshComponent* TerrainMesh3Component = new StaticMesh("resources/assets/level/20degrees.fbx");
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

		MeshComponent* TerrainMesh4Component = new StaticMesh("resources/assets/level/35degrees.fbx");
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
					// POINT LIGHTS
		std::vector<Light*> Lights;

		// point light positions
		glm::vec3 pointLightPositions[] = {
			glm::vec3(3.5f,  1.2f,  2.0f),
			glm::vec3(0.5f,  0.2f,  -2.0f),
			glm::vec3(-3.5f,  1.2f,  4.0f),
			glm::vec3(-4.5f,  2.2f,  4.0f),
			glm::vec3(-5.5f,  3.2f,  4.0f),
			glm::vec3(-6.5f,  4.2f,  4.0f),
			glm::vec3(-7.5f,  5.2f,  4.0f),
			glm::vec3(-1.5f,  5.2f,  4.0f),
			glm::vec3(-7.5f,  5.2f,  4.0f)

		};
		// point lights
		for (glm::vec3 pos : pointLightPositions)
		{
			Light* pointLight = new Light(pos, Chroma::Type::Light::kPointLight);
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Chroma::Type::Light::kSunlight, glm::vec3(0.0, -1.0, -0.1), 2.0f);
		Sun->SetDiffuse(glm::vec3(1.0));
		Sun->SetIntensity(3.0);
		Lights.push_back(Sun);
		Chroma::Scene::SetLights(Lights);


		// ____________________________________________________
		// Entities
		// ____________________________________________________
		IEntity* SponzaEntity = new Entity;
		SponzaEntity->SetName("Sponza");
		SponzaEntity->SetScale(glm::vec3(0.01));
		Chroma::Scene::AddEntity(SponzaEntity);


		// ____________________________________________________
		// SHADERS
		// ____________________________________________________

		Shader PBRAlphaShader("resources/shaders/fragSSSS.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader SemiTransparentShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRShader("resources/shaders/fragSSSS.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRSkinShader("resources/shaders/fragSSSS.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");

		// ____________________________________________________
		// TEXTURES
		// ____________________________________________________

		// Default
		// Generic




		// VASE GROUND
		// flowers


		Material vaseGroundPlant;






		// ____________________________________________________
		// ENTITIES
		// ____________________________________________________

		IEntity* HumanEntity = new Entity;
		HumanEntity->SetName("Human Model");
		Chroma::Scene::AddEntity(HumanEntity);
		HumanEntity->SetScale(glm::vec3(40.0f));
		HumanEntity->SetTranslation(glm::vec3(-6.8, 0, 0));


		// Head
		Texture headAlbedo = Texture("resources/human/textures/head/head_albedo.jpg", Chroma::Type::Texture::kAlbedo);
		Texture headNormal = Texture("resources/human/textures/head/head_normal.jpg", Chroma::Type::Texture::kNormal);
		Texture headMetRoughAO = Texture("resources/human/textures/head/head_metroughao.jpg");
		headMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);
		Texture headTranslucency = Texture("resources/human/textures/head/head_translucency.jpg");
		headTranslucency.SetType(Chroma::Type::Texture::kTranslucency);

		// ____________________________________________________
		// MODELS
		// ____________________________________________________
		// HEAD		
		Material HeadMeshMaterial;
		HeadMeshMaterial.SetShader(PBRAlphaShader);
		HeadMeshMaterial.SetIsLit(false);
		HeadMeshMaterial.SetIsForwardLit(true);
		HeadMeshMaterial.AddTexture(headAlbedo);
		HeadMeshMaterial.AddTexture(headNormal);
		HeadMeshMaterial.AddTexture(headMetRoughAO);
		HeadMeshMaterial.AddTexture(headTranslucency);

		MeshComponent* HeadMeshComponent = new StaticMesh("resources/human/Head/Head.fbx");
		HeadMeshComponent->SetMaterial(HeadMeshMaterial);
		HumanEntity->AddComponent(HeadMeshComponent);

		// Eyelashes 
		MeshComponent* EyelashesMeshComponent = new StaticMesh("resources/human/Head/Eyelashes.fbx");
		HumanEntity->AddComponent(EyelashesMeshComponent);

		// Eyebrows
		MeshComponent* EyebrowsMeshComponent = new StaticMesh("resources/human/Head/Eyebrows.fbx");
		HumanEntity->AddComponent(EyebrowsMeshComponent);


	


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
			Light* pointLight = new Light(pos, Chroma::Type::Light::kPointLight);
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Chroma::Type::Light::kSunlight, glm::vec3(-10.0, -1.0, -0.1), 2.0f);
		Sun->SetDiffuse(glm::vec3(1.0));
		Sun->SetIntensity(3.0);
		Lights.push_back(Sun);
		Chroma::Scene::SetLights(Lights);


		// ____________________________________________________
		// SHADERS
		// ____________________________________________________

		//Shader SemiTransparentShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		//Shader PBRSkinShaderExperimental("resources/shaders/fragSSSS.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		//Shader PBRSkinShader("resources/shaders/fragSSSS_backup.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRShader("resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");

		// ____________________________________________________
		// TEXTURES
		// ____________________________________________________

		// Default
		// Generic
		Texture greyAlbedo("resources/textures/colors/grey.jpg", Chroma::Type::Texture::kAlbedo);

		//Texture gridAlbedo("resources/animation/textures/grid.jpg");
		
		Texture flatNormal("resources/textures/test/flat_normal.jpg", Chroma::Type::Texture::kNormal);
		

		// Lookdev Sphere
		Texture lookDevAlbedo = Texture("resources/textures/pbr/lookdev_pbr/albedo.jpg", Chroma::Type::Texture::kAlbedo);
		Texture lookDevNormal = Texture("resources/textures/pbr/lookdev_pbr/normal.jpg", Chroma::Type::Texture::kNormal);

		Texture lookDevMetRoughAO = Texture("resources/textures/pbr/lookdev_pbr/MetRoughAO.jpg", Chroma::Type::Texture::kMetRoughAO);
		lookDevMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);

		// ____________________________________________________
		// Materials
		// ____________________________________________________

		Material testMat;
		testMat.SetShader(PBRShader);
		testMat.AddTexture(lookDevAlbedo);
		testMat.AddTexture(lookDevNormal);
		testMat.AddTexture(lookDevMetRoughAO);

		Material testMat2;
		testMat2.AddTexture(greyAlbedo);
		testMat2.AddTexture(lookDevNormal);
		testMat2.AddTexture(lookDevMetRoughAO);

		Material testMat3;
		Material testMat4;
		Material testMat5;

		std::vector<Material> materialList{ testMat, testMat2 , testMat3, testMat4, testMat5 };
		//std::vector<Material> materialList{ testMat};

		float spacing{ 3.0 };

		//IEntity* lookDevEntity = new Entity;
		//lookDevEntity->SetName("LookDev Sphere : ");
		//Chroma::Scene::AddEntity(lookDevEntity);
		//lookDevEntity->SetScale(glm::vec3(1.5f));

		for (unsigned int i = 0; i < materialList.size(); i++)
		{
			IEntity* lookDevEntity = new Entity;
			lookDevEntity->SetName("LookDev Sphere : " + std::to_string(i));
			Chroma::Scene::AddEntity(lookDevEntity);
			//lookDevEntity->SetScale(glm::vec3(0.2f));
			lookDevEntity->SetTranslation(glm::vec3((float)i * spacing, 2.1, 0));
			// mesh component
			MeshComponent* lookDevMeshComponent = new StaticMesh("resources/lookdev/sphere.obj");
			lookDevMeshComponent->SetMaterial(testMat);
			//lookDevMeshComponent->SetTranslation(glm::vec3((float)i * spacing, 2.1, 0));
			lookDevEntity->AddComponent(lookDevMeshComponent);

			// LookDev Physics
			PhysicsComponent* LookDevPhysicsComponent = new PhysicsComponent();
			LookDevPhysicsComponent->SetColliderShape(Box);
			LookDevPhysicsComponent->SetCollisionState(Kinematic);
			lookDevEntity->AddComponent(LookDevPhysicsComponent);
		}

	}

	void Editor::Test(std::vector<Texture*>& textures)
	{

		// SUNLIGHT
		IComponent* Sun = new Light(Chroma::Type::Light::kSunlight, glm::vec3(-10.0, -1.0, -0.1), 2.0f);
		/*Sun->SetDiffuse(glm::vec3(1.0));
		Sun->SetIntensity(3.0);*/

		Chroma::Scene::AddLight(Sun);


		std::vector<std::string> texturePaths = {
	"resources/lookdev/Sponza/textures_final/vase_ground_Albedo.png",
	"resources/lookdev/Sponza/textures_final/vase_ground_MetRoughAO.png",
	"resources/lookdev/Sponza/textures_final/vase_ground_transparency.png",
	"resources/lookdev/Sponza/textures_final/arch_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/arch_Normal.jpg",
	"resources/lookdev/Sponza/textures_final/bricks_Albedo.png",
	"resources/lookdev/Sponza/textures_final/bricks_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/bricks_Normal.jpg",
	"resources/lookdev/Sponza/textures_final/columnA_Albedo.png",
	"resources/lookdev/Sponza/textures_final/columnA_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/columnA_Normal.jpg",
	"resources/lookdev/Sponza/textures_final/ground_Albedo.png",
	"resources/lookdev/Sponza/textures_final/ground_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/ground_Normal.jpg",
	"resources/lookdev/Sponza/textures_final/lion_Albedo.png",
	"resources/lookdev/Sponza/textures_final/lion_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/lion_Normal.jpg",
	"resources/lookdev/Sponza/textures_final/roof_Albedo.png",
	"resources/lookdev/Sponza/textures_final/roof_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/roof_Normal.",
	"resources/lookdev/Sponza/textures_final/vase_ground_normal.jpg",
	"resources/lookdev/Sponza/textures_final/vase_groundBase_MetRoughAO.png",
	"resources/lookdev/Sponza/textures_final/vase_groundBase_MetRoughAO_Albedo.png",
		"resources/lookdev/Sponza/textures_final/vase_ground_Albedo.png",
	"resources/lookdev/Sponza/textures_final/vase_ground_MetRoughAO.png",
	"resources/lookdev/Sponza/textures_final/vase_ground_transparency.png",
	"resources/lookdev/Sponza/textures_final/arch_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/arch_Normal.jpg",
	"resources/lookdev/Sponza/textures_final/bricks_Albedo.png",
	"resources/lookdev/Sponza/textures_final/bricks_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/bricks_Normal.jpg",
	"resources/lookdev/Sponza/textures_final/columnA_Albedo.png",
	"resources/lookdev/Sponza/textures_final/columnA_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/columnA_Normal.jpg",
	"resources/lookdev/Sponza/textures_final/ground_Albedo.png",
	"resources/lookdev/Sponza/textures_final/ground_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/ground_Normal.jpg",
	"resources/lookdev/Sponza/textures_final/lion_Albedo.png",
	"resources/lookdev/Sponza/textures_final/lion_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/lion_Normal.jpg",
	"resources/lookdev/Sponza/textures_final/roof_Albedo.png",
	"resources/lookdev/Sponza/textures_final/roof_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/roof_Normal.",
	"resources/lookdev/Sponza/textures_final/vase_ground_normal.jpg",
	"resources/lookdev/Sponza/textures_final/vase_groundBase_MetRoughAO.png",
	"resources/lookdev/Sponza/textures_final/vase_groundBase_MetRoughAO_Albedo.png",
		"resources/lookdev/Sponza/textures_final/vase_ground_Albedo.png",
	"resources/lookdev/Sponza/textures_final/vase_ground_MetRoughAO.png",
	"resources/lookdev/Sponza/textures_final/vase_ground_transparency.png",
	"resources/lookdev/Sponza/textures_final/arch_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/arch_Normal.jpg",
	"resources/lookdev/Sponza/textures_final/bricks_Albedo.png",
	"resources/lookdev/Sponza/textures_final/bricks_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/bricks_Normal.jpg",
	"resources/lookdev/Sponza/textures_final/columnA_Albedo.png",
	"resources/lookdev/Sponza/textures_final/columnA_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/columnA_Normal.jpg",
	"resources/lookdev/Sponza/textures_final/ground_Albedo.png",
	"resources/lookdev/Sponza/textures_final/ground_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/ground_Normal.jpg",
	"resources/lookdev/Sponza/textures_final/lion_Albedo.png",
	"resources/lookdev/Sponza/textures_final/lion_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/lion_Normal.jpg",
	"resources/lookdev/Sponza/textures_final/roof_Albedo.png",
	"resources/lookdev/Sponza/textures_final/roof_MetRoughAO.jpg",
	"resources/lookdev/Sponza/textures_final/roof_Normal.",
	"resources/lookdev/Sponza/textures_final/vase_ground_normal.jpg",
	"resources/lookdev/Sponza/textures_final/vase_groundBase_MetRoughAO.png",
	"resources/lookdev/Sponza/textures_final/vase_groundBase_MetRoughAO_Albedo.png",
		};


		for (const std::string& path : texturePaths)
		{
			Texture* newTX = new Texture(path);
			textures.push_back(newTX);
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
			Light* pointLight = new Light(pos, Chroma::Type::Light::kPointLight);
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Chroma::Type::Light::kSunlight, glm::vec3(-10.0, -1.0, -0.1), 2.0f);
		Sun->SetDiffuse(glm::vec3(1.0));
		Sun->SetIntensity(3.0);
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
		Texture greyAlbedo("resources/textures/colors/grey.jpg", Chroma::Type::Texture::kAlbedo);
		Texture gridAlbedo("resources/animation/textures/grid.jpg", Chroma::Type::Texture::kAlbedo);
		Texture flatNormal("resources/textures/test/flat_normal.jpg");

		// Lookdev Sphere
		Texture lookDevAlbedo = Texture("resources/textures/pbr/lookdev_pbr/albedo.jpg", Chroma::Type::Texture::kAlbedo);
		Texture lookDevNormal = Texture("resources/textures/pbr/lookdev_pbr/normal.jpg", Chroma::Type::Texture::kNormal);
		Texture lookDevMetRoughAO = Texture("resources/textures/pbr/lookdev_pbr/MetRoughAO.jpg");
		lookDevMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);

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
			//glm::vec3(3.5f,  1.2f,  2.0f),
			//glm::vec3(0.5f,  0.2f,  -2.0f),
			//glm::vec3(-3.5f,  1.2f,  4.0f),
			//glm::vec3(-4.5f,  2.2f,  4.0f),
			//glm::vec3(-5.5f,  3.2f,  4.0f),
			//glm::vec3(-6.5f,  4.2f,  4.0f),
			//glm::vec3(-7.5f,  5.2f,  4.0f),
			glm::vec3(-9.5f,  5.2f,  4.0f),
			glm::vec3(-7.5f,  5.2f,  4.0f),
			glm::vec3(-3.5f,  5.2f,  5.0f),
			glm::vec3(-7.5f,  5.2f,  5.0f)

		};
		// point lights
		for (glm::vec3 pos : pointLightPositions)
		{
			Light* pointLight = new Light(pos, Chroma::Type::Light::kPointLight);
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Chroma::Type::Light::kSunlight, glm::vec3(0.0, -1.0, -0.1), 2.0f);
		Sun->SetDiffuse(glm::vec3(1.0));
		Sun->SetIntensity(3.0);
		Lights.push_back(Sun);
		Chroma::Scene::SetLights(Lights);


		// ____________________________________________________
		// Entities
		// ____________________________________________________
		IEntity* SponzaEntity = new Entity;
		SponzaEntity->SetName("Sponza");
		SponzaEntity->SetScale(glm::vec3(0.01));
		Chroma::Scene::AddEntity(SponzaEntity);


		// ____________________________________________________
		// SHADERS
		// ____________________________________________________

		Shader SemiTransparentShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRShader("resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		Shader PBRAlphaShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");

		// ____________________________________________________
		// TEXTURES
		// ____________________________________________________

		// Default
		// Generic
		Texture greyAlbedo("resources/textures/colors/grey.jpg", Chroma::Type::Texture::kAlbedo);
		Texture gridAlbedo("resources/animation/textures/grid.jpg", Chroma::Type::Texture::kAlbedo);
		Texture flatNormal("resources/textures/test/flat_normal.jpg", Chroma::Type::Texture::kNormal);


		// CLOTH
		//red
		Texture materialRedAlbedo("resources/lookdev/Sponza/textures/sponza_fabric_diff.png", Chroma::Type::Texture::kAlbedo);
		Texture materialNormal("resources/lookdev/Sponza/textures/sponza_fabric_diff_NRM.jpg", Chroma::Type::Texture::kNormal);


		Material RedClothMaterial;
		RedClothMaterial.SetShader(PBRShader);
		RedClothMaterial.AddTexture(materialRedAlbedo);
		RedClothMaterial.AddTexture(materialNormal);

		MeshComponent* SponzaMaterialMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_cloth_red.fbx");
		SponzaMaterialMeshComponent->SetMaterial(RedClothMaterial);
		SponzaEntity->AddComponent(SponzaMaterialMeshComponent);


		//green
		Texture materialGreenAlbedo("resources/lookdev/Sponza/textures/sponza_fabric_green_diff.png", Chroma::Type::Texture::kAlbedo);

		Material GreenClothMaterial;
		GreenClothMaterial.SetShader(PBRShader);
		GreenClothMaterial.AddTexture(materialGreenAlbedo);
		GreenClothMaterial.AddTexture(materialNormal);

		MeshComponent* SponzaGreenMaterialMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_cloth_green.fbx");
		SponzaGreenMaterialMeshComponent->SetMaterial(GreenClothMaterial);
		SponzaEntity->AddComponent(SponzaGreenMaterialMeshComponent);


		//blue
		Texture materialBlueAlbedo("resources/lookdev/Sponza/textures/sponza_fabric_blue_diff.png", Chroma::Type::Texture::kAlbedo);

		Material BlueClothMaterial;
		BlueClothMaterial.SetShader(PBRShader);
		BlueClothMaterial.AddTexture(materialBlueAlbedo);
		BlueClothMaterial.AddTexture(materialNormal);

		MeshComponent* SponzaBlueMaterialMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_cloth_blue.fbx");
		SponzaBlueMaterialMeshComponent->SetMaterial(BlueClothMaterial);
		SponzaEntity->AddComponent(SponzaBlueMaterialMeshComponent);


		// VASE GROUND
		// flowers
		Texture vaseGroundFlowerAlbedo("resources/lookdev/Sponza/textures_final/vase_ground_Albedo.png", Chroma::Type::Texture::kAlbedo);
		Texture vaseGroundFlowerNormal("resources/lookdev/Sponza/textures_final/vase_ground_normal.jpg", Chroma::Type::Texture::kNormal);
		Texture vaseGroundFlowerMetRoughAO("resources/lookdev/Sponza/textures_final/vase_ground_MetRoughAO.png");
		vaseGroundFlowerMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);
		Texture vaseGroundFlowerTransp("resources/lookdev/Sponza/textures_final/vase_ground_transparency.png");
		vaseGroundFlowerTransp.SetType(Chroma::Type::Texture::kTranslucency);

		Material vaseGroundPlant;
		vaseGroundPlant.SetShader(PBRAlphaShader);
		vaseGroundPlant.AddTexture(vaseGroundFlowerAlbedo);
		vaseGroundPlant.AddTexture(vaseGroundFlowerNormal);
		vaseGroundPlant.AddTexture(vaseGroundFlowerMetRoughAO);
		vaseGroundPlant.AddTexture(vaseGroundFlowerTransp);

		// Mesh component
		MeshComponent* VaseFlowerMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_vase_ground_plants.obj");
		VaseFlowerMeshComponent->SetMaterial(vaseGroundPlant);
		//VaseFlowerMeshComponent->SetIsDoubleSided(true);
		VaseFlowerMeshComponent->SetIsLit(false);
		//VaseFlowerMeshComponent->SetIsTransparent(true);
		VaseFlowerMeshComponent->SetIsForwardLit(true);
		SponzaEntity->AddComponent(VaseFlowerMeshComponent);

		// vase
		Texture vaseGroundAlbedo("resources/lookdev/Sponza/textures_final/vase_groundBase_MetRoughAO_Albedo.png", Chroma::Type::Texture::kAlbedo);
		Texture vaseGroundNormal("resources/lookdev/Sponza/textures_final/vase_groundBase_normal.jpg", Chroma::Type::Texture::kNormal);
		Texture vaseGroundMetRoughAO("resources/lookdev/Sponza/textures_final/vase_groundBase_MetRoughAO.png");
		vaseGroundMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);

		Material vaseGroundMat;
		vaseGroundMat.AddTexture(vaseGroundAlbedo);
		vaseGroundMat.AddTexture(vaseGroundNormal);
		vaseGroundMat.AddTexture(vaseGroundMetRoughAO);

		// Mesh component
		MeshComponent* SponzaVaseComponent = new StaticMesh("resources/lookdev/Sponza/sponza_vase_ground_base.obj");
		SponzaVaseComponent->SetMaterial(vaseGroundMat);
		SponzaEntity->AddComponent(SponzaVaseComponent);

		// GROUND
		Texture groundAlbedo("resources/lookdev/Sponza/textures_final/ground_Albedo.png", Chroma::Type::Texture::kAlbedo);
		Texture groundNormal("resources/lookdev/Sponza/textures_final/ground_Normal.jpg", Chroma::Type::Texture::kNormal);
		Texture groundMetRoughAO("resources/lookdev/Sponza/textures_final/ground_MetRoughAO.jpg");
		groundMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);

		Material groundMat;
		groundMat.AddTexture(groundAlbedo);
		groundMat.AddTexture(groundNormal);
		groundMat.AddTexture(groundMetRoughAO);

		// Mesh component
		MeshComponent* GroundMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_ground.fbx");
		GroundMeshComponent->SetMaterial(groundMat);
		SponzaEntity->AddComponent(GroundMeshComponent);

		// ARCHES
		Texture archAlbedo("resources/lookdev/Sponza/textures_final/arch_Albedo.png", Chroma::Type::Texture::kAlbedo);
		Texture archNormal("resources/lookdev/Sponza/textures_final/arch_Normal.jpg", Chroma::Type::Texture::kNormal);
		Texture archMetRoughAO("resources/lookdev/Sponza/textures_final/arch_MetRoughAO.jpg");
		archMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);

		Material archMat;
		archMat.AddTexture(archAlbedo);
		archMat.AddTexture(archNormal);
		archMat.AddTexture(archMetRoughAO);

		// Mesh component
		MeshComponent* ArchMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_arches.fbx");
		ArchMeshComponent->SetMaterial(archMat);
		SponzaEntity->AddComponent(ArchMeshComponent);

		// ROOF
		Texture roofAlbedo("resources/lookdev/Sponza/textures_final/roof_Albedo.png", Chroma::Type::Texture::kAlbedo);
		Texture roofNormal("resources/lookdev/Sponza/textures_final/roof_Normal.jpg", Chroma::Type::Texture::kNormal);
		Texture roofMetRoughAO("resources/lookdev/Sponza/textures_final/roof_MetRoughAO.jpg");
		roofMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);

		Material roofMat;
		roofMat.AddTexture(roofAlbedo);
		roofMat.AddTexture(roofNormal);
		roofMat.AddTexture(roofMetRoughAO);

		// Mesh component
		MeshComponent* roofMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_roof.fbx");
		roofMeshComponent->SetMaterial(roofMat);
		SponzaEntity->AddComponent(roofMeshComponent);

		// WALLS
		Texture wallsAlbedo("resources/lookdev/Sponza/textures_final/bricks_Albedo.png", Chroma::Type::Texture::kAlbedo);
		Texture wallsNormal("resources/lookdev/Sponza/textures_final/bricks_Normal.jpg", Chroma::Type::Texture::kNormal);
		Texture wallsMetRoughAO("resources/lookdev/Sponza/textures_final/bricks_MetRoughAO.jpg");
		wallsMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);

		Material wallsMat;
		wallsMat.AddTexture(wallsAlbedo);
		wallsMat.AddTexture(wallsNormal);
		wallsMat.AddTexture(wallsMetRoughAO);

		// Mesh component
		MeshComponent* WallsMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_walls.fbx");
		WallsMeshComponent->SetMaterial(wallsMat);
		SponzaEntity->AddComponent(WallsMeshComponent);

		// LION
		Texture lionAlbedo("resources/lookdev/Sponza/textures_final/lion_Albedo.png", Chroma::Type::Texture::kAlbedo);
		Texture lionNormal("resources/lookdev/Sponza/textures_final/lion_Normal.jpg", Chroma::Type::Texture::kNormal);
		Texture lionMetRoughAO("resources/lookdev/Sponza/textures_final/lion_MetRoughAO.jpg");
		lionMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);

		Material lionMat;
		lionMat.AddTexture(lionAlbedo);
		lionMat.AddTexture(lionNormal);
		lionMat.AddTexture(lionMetRoughAO);

		// Mesh component
		MeshComponent* lionMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_LionHead.fbx");
		lionMeshComponent->SetMaterial(lionMat);
		SponzaEntity->AddComponent(lionMeshComponent);

		// COLUMNS
		Texture columnAAlbedo("resources/lookdev/Sponza/textures_final/columnA_Albedo.png", Chroma::Type::Texture::kAlbedo);
		Texture columnANormal("resources/lookdev/Sponza/textures_final/columnA_Normal.jpg", Chroma::Type::Texture::kNormal);
		Texture columnAMetRoughAO("resources/lookdev/Sponza/textures_final/columnA_MetRoughAO.jpg");
		columnAMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);

		Material columnAMat;
		columnAMat.AddTexture(columnAAlbedo);
		columnAMat.AddTexture(columnANormal);
		columnAMat.AddTexture(columnAMetRoughAO);

		// Mesh component
		MeshComponent* columnAMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_columns_ground.fbx");
		columnAMeshComponent->SetMaterial(columnAMat);
		SponzaEntity->AddComponent(columnAMeshComponent);

		// Mesh component
		MeshComponent* columnsTopFloorBAMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_columnsB_firstFloor.fbx");
		columnsTopFloorBAMeshComponent->SetMaterial(columnAMat);
		SponzaEntity->AddComponent(columnsTopFloorBAMeshComponent);




		// CURTAINS

		// RED
		Texture curtainRedAlbedo("resources/lookdev/Sponza/textures_final/curtain_red_Albedo.png", Chroma::Type::Texture::kAlbedo);
		Texture curtainRedNormal("resources/lookdev/Sponza/textures_final/columnA_Normal.jpg", Chroma::Type::Texture::kNormal);
		Texture curtainRedMetRoughAO("resources/lookdev/Sponza/textures_final/columnA_MetRoughAO.jpg", Chroma::Type::Texture::kMetRoughAO);

		Material curtainRedMaterial;
		curtainRedMaterial.AddTexture(curtainRedAlbedo);
		curtainRedMaterial.AddTexture(curtainRedNormal);
		curtainRedMaterial.AddTexture(curtainRedMetRoughAO);

		// Mesh component
		MeshComponent* curtainRedMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_curtain_red.fbx");
		curtainRedMeshComponent->SetMaterial(curtainRedMaterial);
		SponzaEntity->AddComponent(curtainRedMeshComponent);


		// BLUE
		Texture curtainBlueAlbedo("resources/lookdev/Sponza/textures_final/curtain_blue_Albedo.png", Chroma::Type::Texture::kAlbedo);
		Texture curtainBlueNormal("resources/lookdev/Sponza/textures_final/columnA_Normal.jpg", Chroma::Type::Texture::kNormal);
		Texture curtainBlueMetRoughAO("resources/lookdev/Sponza/textures_final/columnA_MetRoughAO.jpg", Chroma::Type::Texture::kMetRoughAO);

		Material curtainBlueMaterial;
		curtainBlueMaterial.AddTexture(curtainBlueAlbedo);
		curtainBlueMaterial.AddTexture(curtainBlueNormal);
		curtainBlueMaterial.AddTexture(curtainBlueMetRoughAO);

		// Mesh component
		MeshComponent* curtainBlueMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_curtain_blue.fbx");
		curtainBlueMeshComponent->SetMaterial(curtainBlueMaterial);
		SponzaEntity->AddComponent(curtainBlueMeshComponent);


		// Green
		Texture curtainGreenAlbedo("resources/lookdev/Sponza/textures_final/curtain_green_Albedo.png", Chroma::Type::Texture::kAlbedo);
		Texture curtainGreenNormal("resources/lookdev/Sponza/textures_final/columnA_Normal.jpg", Chroma::Type::Texture::kNormal);
		Texture curtainGreenMetRoughAO("resources/lookdev/Sponza/textures_final/columnA_MetRoughAO.jpg", Chroma::Type::Texture::kMetRoughAO);

		Material curtainGreenMaterial;
		curtainGreenMaterial.AddTexture(curtainGreenAlbedo);
		curtainGreenMaterial.AddTexture(curtainGreenNormal);
		curtainGreenMaterial.AddTexture(curtainGreenMetRoughAO);

		// Mesh component
		MeshComponent* curtainGreenMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_curtain_green.fbx");
		curtainGreenMeshComponent->SetMaterial(curtainGreenMaterial);
		SponzaEntity->AddComponent(curtainGreenMeshComponent);

		// COLUMNS Top floor
		Texture ColumnsTopFloorAlbedo("resources/lookdev/Sponza/textures_final/curtain_green_Albedo.png", Chroma::Type::Texture::kAlbedo);
		Texture ColumnsTopFloorNormal("resources/lookdev/Sponza/textures_final/columnA_Normal.jpg", Chroma::Type::Texture::kNormal);
		Texture ColumnsTopFloorMetRoughAO("resources/lookdev/Sponza/textures_final/columnA_MetRoughAO.jpg", Chroma::Type::Texture::kMetRoughAO);

		Material ColumnsTopFloorMaterial;
		ColumnsTopFloorMaterial.AddTexture(ColumnsTopFloorAlbedo);
		ColumnsTopFloorMaterial.AddTexture(ColumnsTopFloorNormal);
		ColumnsTopFloorMaterial.AddTexture(ColumnsTopFloorMetRoughAO);

		// Mesh component
		MeshComponent* ColumnsTopFloorMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_columns_firstFloor.fbx");
		ColumnsTopFloorMeshComponent->SetMaterial(columnAMat);
		SponzaEntity->AddComponent(ColumnsTopFloorMeshComponent);


		// CEILINGS
		Texture CeilingsAlbedo("resources/lookdev/Sponza/textures_final/ceiling_Albedo.png", Chroma::Type::Texture::kAlbedo);
		Texture CeilingsNormal("resources/lookdev/Sponza/textures_final/ceiling_Normal.jpg", Chroma::Type::Texture::kNormal);
		Texture CeilingsMetRoughAO("resources/lookdev/Sponza/textures_final/ceiling_MetRoughAO.jpg", Chroma::Type::Texture::kMetRoughAO);

		Material CeilingsMaterial;
		CeilingsMaterial.AddTexture(CeilingsAlbedo);
		CeilingsMaterial.AddTexture(CeilingsNormal);
		CeilingsMaterial.AddTexture(CeilingsMetRoughAO);

		// Mesh component
		MeshComponent* CeilingsMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponza_ceilings.fbx");
		CeilingsMeshComponent->SetMaterial(CeilingsMaterial);
		SponzaEntity->AddComponent(CeilingsMeshComponent);


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
			Light* pointLight = new Light(pos, Chroma::Type::Light::kPointLight);
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Chroma::Type::Light::kSunlight, glm::vec3(-10.0, -1.0, -0.1), 2.0f);
		Sun->SetDiffuse(glm::vec3(1.0));
		Sun->SetIntensity(3.0);
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
		Texture greyAlbedo("resources/textures/colors/grey.jpg", Chroma::Type::Texture::kAlbedo);
		Texture gridAlbedo("resources/animation/textures/grid.jpg", Chroma::Type::Texture::kAlbedo);
		Texture flatNormal("resources/textures/test/flat_normal.jpg", Chroma::Type::Texture::kNormal);

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
			Light* pointLight = new Light(pos, Chroma::Type::Light::kPointLight);
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Chroma::Type::Light::kSunlight, glm::vec3(-10.0, -1.0, -0.1), 2.0f);
		Sun->SetDiffuse(glm::vec3(1.0));
		Sun->SetIntensity(3.0);
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
		Texture gridAlbedo("resources/animation/textures/grid.jpg");
		Texture flatNormal("resources/textures/test/flat_normal.jpg", Chroma::Type::Texture::kNormal);
		Texture lookDevMetRoughAO = Texture("resources/textures/pbr/lookdev_pbr/MetRoughAO.jpg");
		lookDevMetRoughAO.SetType(Chroma::Type::Texture::kMetRoughAO);

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
			Light* pointLight = new Light(pos, Chroma::Type::Light::kPointLight);
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Chroma::Type::Light::kSunlight, glm::vec3(-10.0, -1.0, -0.1), 2.0f);
		Sun->SetDiffuse(glm::vec3(1.0));
		Sun->SetIntensity(3.0);
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
		Texture greyAlbedo("resources/textures/colors/grey.jpg", Chroma::Type::Texture::kAlbedo);
		Texture gridAlbedo("resources/animation/textures/grid.jpg", Chroma::Type::Texture::kAlbedo);
		Texture flatNormal("resources/textures/test/flat_normal.jpg", Chroma::Type::Texture::kNormal);

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

	void Editor::SponzaCombined()
	{
		// POINT LIGHTS
		std::vector<Light*> Lights;

		// point light positions
		glm::vec3 pointLightPositions[] = {
			glm::vec3(3.5f,  1.2f,  2.0f),
			glm::vec3(0.5f,  0.2f,  -2.0f),
			glm::vec3(-3.5f,  1.2f,  4.0f),
			glm::vec3(-3.5f,  1.2f,  5.0f),
		};
		// point lights
		for (glm::vec3 pos : pointLightPositions)
		{
			Light* pointLight = new Light(pos, Chroma::Type::Light::kPointLight);
			Lights.push_back(pointLight);
		}

		// SUNLIGHT
		Light* Sun = new Light(Chroma::Type::Light::kSunlight, glm::vec3(0.0, -1.0, -0.1), 2.0f);
		Sun->SetDiffuse(glm::vec3(1.0));
		Sun->SetIntensity(3.0);
		Lights.push_back(Sun);
		Chroma::Scene::SetLights(Lights);

		// ____________________________________________________
		// TEXTURES
		// ____________________________________________________

		// Default
		// Generic
		Texture greyAlbedo("resources/textures/colors/grey.jpg", Chroma::Type::Texture::kAlbedo);
		Texture gridAlbedo("resources/animation/textures/grid.jpg", Chroma::Type::Texture::kAlbedo);
		Texture flatNormal("resources/textures/test/flat_normal.jpg", Chroma::Type::Texture::kNormal);

		// ____________________________________________________
		// Materials
		// ____________________________________________________

		Material basicMat;
		basicMat.AddTexture(greyAlbedo);
		//basicMat.AddTexture(flatNormal);


		// ____________________________________________________
		// Entities
		// ____________________________________________________
		IEntity* SponzaEntity = new Entity;
		SponzaEntity->SetName("Sponza");
		//SponzaEntity->SetScale(glm::vec3(0.01));
		Chroma::Scene::AddEntity(SponzaEntity);

		// ____________________________________________________
		// Components
		// ____________________________________________________

		// Mesh component
		MeshComponent* SponzaMeshComponent = new StaticMesh("resources/lookdev/Sponza/sponzacombine.fbx");
		SponzaMeshComponent->SetMaterial(basicMat);
		SponzaEntity->AddComponent(SponzaMeshComponent);
	}


	void Editor::OnUpdate()
	{
		// UI
		DrawUI();
	}

	void Editor::DrawUI()
	{
		Chroma::EditorUI::Draw();
	}

	
}