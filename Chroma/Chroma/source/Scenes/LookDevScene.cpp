#if not defined(EDITOR)

//common
#include <common/PrecompiledHeader.h>


// chroma
#include <engine/engine.h>
#include <resources/ResourceManager.h>
#include <terrain/Terrain.h>
#include <ui/uicomponents/Icon.h>

#include <entity/Entity.h>
#include <model/SkinnedMesh.h>



int main()
{
	// INIT CHROMA
	Chroma::Engine::Init();

	// Initialize Scene
	Chroma::Scene::PreSceneBuild();

	// POPULATE SCENE CONTENTS
	// ____________________________________________________
	// LIGHTS
	// ____________________________________________________

	// CONSTS
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
	Shader ColoredShader("resources/shaders/fragBasic.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");

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

	// ____________________________________________________
	// flowers
	Texture vaseGroundFlowerAlbedo("resources/lookdev/Sponza/textures_final/vase_ground_Albedo.png");
	vaseGroundFlowerAlbedo.m_Type = Texture::ALBEDO;
	Texture vaseGroundFlowerNormal("resources/lookdev/Sponza/textures_final/vase_ground_normal.jpg");
	vaseGroundFlowerNormal.m_Type = Texture::NORMAL;
	Texture vaseGroundFlowerMetRoughAO("resources/lookdev/Sponza/textures_final/vase_ground_MetRoughAO.png");
	vaseGroundFlowerMetRoughAO.m_Type = Texture::METROUGHAO;
	Texture vaseGroundFlowerTransp("resources/lookdev/Sponza/textures_final/vase_ground_transparency.png");
	vaseGroundFlowerTransp.m_Type = Texture::TRANSLUCENCY;
	Material plantMat;
	plantMat.SetIsDoubleSided(true);
	plantMat.SetIsTransparent(true);
	plantMat.SetIsForwardLit(true);
	plantMat.SetIsLit(false);
	plantMat.SetUVMultiply(glm::vec2(2.0));
	plantMat.SetShader(PBRSkinShaderExperimental);
	plantMat.AddTexture(vaseGroundFlowerAlbedo);
	plantMat.AddTexture(vaseGroundFlowerNormal);
	plantMat.AddTexture(vaseGroundFlowerMetRoughAO);
	plantMat.AddTexture(vaseGroundFlowerTransp);

	Material testMat;
	testMat.SetShader(PBRShader);
	testMat.AddTexture(lookDevAlbedo);
	testMat.AddTexture(lookDevNormal);
	testMat.AddTexture(lookDevMetRoughAO);

	Material testMat2;
	testMat2.SetIsLit(false);
	testMat2.SetIsForwardLit(true);
	testMat2.SetShader(ColoredShader);
	testMat2.AddUniform("color", glm::vec3(0.5, 0.0, 0.5));
	testMat2.AddTexture(gridAlbedo);
	testMat2.AddTexture(flatNormal);
	Material testMat3;
	Material testMat4;
	Material testMat5;

	//std::vector<Material> materialList{ testMat, testMat2 , testMat3, testMat4, testMat5 };
	std::vector<Material> materialList{ plantMat, testMat2, testMat };

	const float spacing{ 3.0 };

	const float totalSpace = - (materialList.size() * spacing) / 2.0;

	for (unsigned int i = 0; i < materialList.size(); i++)
	{
		IEntity* lookDevEntity = new Entity;
		lookDevEntity->SetName("LookDev Sphere : " + i);
		Chroma::Scene::AddEntity(lookDevEntity);
		//lookDevEntity->SetScale(glm::vec3(0.1f));
		lookDevEntity->SetTranslation(glm::vec3(totalSpace + (spacing * (float)i), 0, 0));
		// mesh component
		MeshComponent* lookDevMeshComponent = new StaticMesh("resources/lookdev/sphere.obj");
		lookDevMeshComponent->SetMaterial(materialList[i]);
		lookDevEntity->AddComponent(lookDevMeshComponent);

		// LookDev Physics
		PhysicsComponent* LookDevPhysicsComponent = new PhysicsComponent();
		LookDevPhysicsComponent->SetColliderShape(Box);
		LookDevPhysicsComponent->SetCollisionState(Kinematic);
		lookDevEntity->AddComponent(LookDevPhysicsComponent);
	}


	// TERRAIN
	// ____________________________________________________

	Texture groundAlbedo("resources/lookdev/Sponza/textures_final/ground_Albedo.png");
	groundAlbedo.m_Type = Texture::ALBEDO;
	Texture groundNormal("resources/lookdev/Sponza/textures_final/ground_Normal.jpg");
	groundNormal.m_Type = Texture::NORMAL;
	Texture groundMetRoughAO("resources/lookdev/Sponza/textures_final/ground_MetRoughAO.jpg");
	groundMetRoughAO.m_Type = Texture::METROUGHAO;

	Material groundMat;
	//groundMat.AddTexture(groundAlbedo);
	groundMat.AddTexture(groundNormal);
	groundMat.AddTexture(groundMetRoughAO);
	groundMat.SetUVMultiply(10.0 * 4);

	IEntity* TerrainEntity = new Entity;
	Chroma::Scene::AddEntity(TerrainEntity);
	MeshComponent* TerrainMeshComponent = new Terrain;
	TerrainMeshComponent->SetMaterial(groundMat);
	TerrainEntity->AddComponent(TerrainMeshComponent);
	TerrainEntity->SetScale(glm::vec3(10.0, 1.0, 10.0));
	// ____________________________________________________


	// SCENE BUILD

	Chroma::Scene::GetRenderCamera()->SetPosition(glm::vec3(0.0, 4.0, -totalSpace * 2.5));

	Chroma::Scene::PostSceneBuild();


	// RENDER LOOP
	// -----------
	while (Chroma::Screen::IsRunning())
	{
		//Sunlight Rotation	
		Chroma::Scene::GetSunLight()->SetTranslation(glm::vec3(std::sin(GAMETIME * SUNLIGHT_SPIN_SPEED) * SUNLIGHT_DISTANCE, SUNLIGHT_DISTANCE, std::cos(GAMETIME * SUNLIGHT_SPIN_SPEED) * SUNLIGHT_DISTANCE));
		Sun->setDirection(-normalize(Sun->GetPosition()));

		// IBL Switching
		if (Chroma::Input::IsPressed(Chroma::Input::I))
			Chroma::Scene::GetIBL()->LoadIBL("resources/textures/ibl/newportloft_ibl/newportloft.hdr");
		if (Chroma::Input::IsPressed(Chroma::Input::O))
			Chroma::Scene::GetIBL()->LoadIBL("resources/textures/ibl/ditchriver_ibl/river_sharp.hdr");
		if (Chroma::Input::IsPressed(Chroma::Input::P))
			Chroma::Scene::GetIBL()->LoadIBL("resources/textures/ibl/grey_ibl/grey.hdr");

		//Chroma::Render::GetDebugBuffer()->DrawGrid(50, glm::vec3(0.5));
		// GAME TICK
		Chroma::Engine::Tick();

	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	Chroma::Screen::Close();
	return 0;
}

#endif