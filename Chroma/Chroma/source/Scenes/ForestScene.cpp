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



int main6()
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

	// Grass
	Shader foliageShader("resources/shaders/fragVegetation.glsl", "resources/shaders/vertexLitVegetation.glsl");
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

	// Noise
	//Texture noiseTexture = Chroma::ResourceManager::LoadTexture("resources/textures/noise/noise_01.jpg");

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
	//// Grass
	//Texture vaseGroundFlowerAlbedo("resources/forestClearing/textures/grass_00/albedo.png");
	//vaseGroundFlowerAlbedo.m_Type = Texture::ALBEDO;
	//Texture vaseGroundFlowerNormal("resources/forestClearing/textures/grass_00/normal.jpg");
	//vaseGroundFlowerNormal.m_Type = Texture::NORMAL;
	//Texture vaseGroundFlowerMetRoughAO("resources/forestClearing/textures/grass_00/metRoughAO.jpg");
	//vaseGroundFlowerMetRoughAO.m_Type = Texture::METROUGHAO;
	//Texture vaseGroundFlowerTransp("resources/forestClearing/textures/grass_00/transp.jpg");
	//vaseGroundFlowerTransp.m_Type = Texture::TRANSLUCENCY;

	//Material grassMaterial;
	//grassMaterial.SetIsDoubleSided(true);
	//grassMaterial.SetIsTransparent(true);
	//grassMaterial.SetIsForwardLit(true);
	//grassMaterial.SetIsLit(false);
	//grassMaterial.SetShader(foliageShader);
	//grassMaterial.AddTexture(vaseGroundFlowerAlbedo);
	//grassMaterial.AddTexture(vaseGroundFlowerNormal);
	//grassMaterial.AddTexture(vaseGroundFlowerMetRoughAO);
	//grassMaterial.AddTexture(vaseGroundFlowerTransp);


	//// GRASS
	//IEntity* GrassEntity = new Entity;
	//Chroma::Scene::AddEntity(GrassEntity);
	//MeshComponent* grassMeshComponent = new StaticMesh("resources/forestClearing/models/grass_00.fbx");
	//grassMeshComponent->SetMaterial(grassMaterial);
	//GrassEntity->AddComponent(grassMeshComponent);


	// Yellow Flower
	Texture yellowFlowerAlbedo("resources/forestClearing/textures/flower_00/albedo.png");
	yellowFlowerAlbedo.m_Type = Texture::ALBEDO;
	Texture yellowFlowerNormal("resources/forestClearing/textures/flower_00/normal.jpg");
	yellowFlowerNormal.m_Type = Texture::NORMAL;
	Texture yellowFlowerMetRoughAO("resources/forestClearing/textures/flower_00/metRoughAO.jpg");
	yellowFlowerMetRoughAO.m_Type = Texture::METROUGHAO;
	Texture yellowFlowerTransp("resources/forestClearing/textures/flower_00/transp.jpg");
	yellowFlowerTransp.m_Type = Texture::TRANSLUCENCY;

	Material yellowFlowerMaterial;
	yellowFlowerMaterial.SetIsDoubleSided(true);
	yellowFlowerMaterial.SetIsTransparent(true);
	yellowFlowerMaterial.SetIsForwardLit(true);
	yellowFlowerMaterial.SetIsLit(false);
	yellowFlowerMaterial.SetUsesSceneNoise(true);
	yellowFlowerMaterial.SetUsesGameTime(true);
	//yellowFlowerMaterial.SetUVMultiply(glm::vec2(2.0));
	yellowFlowerMaterial.SetShader(foliageShader);
	yellowFlowerMaterial.AddTexture(yellowFlowerAlbedo);
	yellowFlowerMaterial.AddTexture(yellowFlowerNormal);
	yellowFlowerMaterial.AddTexture(yellowFlowerMetRoughAO);
	yellowFlowerMaterial.AddTexture(yellowFlowerTransp);


	// YellowFlower
	IEntity* yellowFlowerEntity = new Entity;
	Chroma::Scene::AddEntity(yellowFlowerEntity);
	MeshComponent* yellowFlowerMeshComponent = new StaticMesh("resources/forestClearing/models/flower_00.fbx");
	yellowFlowerMeshComponent->SetMaterial(yellowFlowerMaterial);
	yellowFlowerEntity->AddComponent(yellowFlowerMeshComponent);

	// CLOVERS
	Texture cloversAlbedo("resources/forestClearing/textures/clovers_00/albedo.png");
	cloversAlbedo.m_Type = Texture::ALBEDO;
	Texture cloversNormal("resources/forestClearing/textures/clovers_00/normal.jpg");
	cloversNormal.m_Type = Texture::NORMAL;
	Texture cloversMetRoughAO("resources/forestClearing/textures/clovers_00/metRoughAO.jpg");
	cloversMetRoughAO.m_Type = Texture::METROUGHAO;
	Texture cloversTransp("resources/forestClearing/textures/clovers_00/transp.jpg");
	cloversTransp.m_Type = Texture::TRANSLUCENCY;

	Material cloverMaterial;
	cloverMaterial.SetIsDoubleSided(true);
	cloverMaterial.SetIsTransparent(true);
	cloverMaterial.SetIsForwardLit(true);
	cloverMaterial.SetIsLit(false);
	cloverMaterial.SetUsesSceneNoise(true);
	cloverMaterial.SetUsesGameTime(true);
	cloverMaterial.SetShader(foliageShader);
	cloverMaterial.AddTexture(cloversAlbedo);
	cloverMaterial.AddTexture(cloversNormal);
	cloverMaterial.AddTexture(cloversMetRoughAO);
	cloverMaterial.AddTexture(cloversTransp);


	// CLOVERS
	IEntity* CloverEntity = new Entity;
	Chroma::Scene::AddEntity(CloverEntity);
	MeshComponent* cloverMeshComponent = new StaticMesh("resources/forestClearing/models/clovers_00.fbx");
	cloverMeshComponent->SetMaterial(cloverMaterial);
	CloverEntity->AddComponent(cloverMeshComponent);


	// GUARDIAN
	Texture GuardianAlbedo("resources/forestClearing/textures/guardian/albedo.jpg");
	GuardianAlbedo.m_Type = Texture::ALBEDO;
	Texture GuardianNormal("resources/forestClearing/textures/guardian/normal.jpg");
	GuardianNormal.m_Type = Texture::NORMAL;
	Texture GuardianMetRoughAO("resources/forestClearing/textures/guardian/metRoughAO.jpg");
	GuardianMetRoughAO.m_Type = Texture::METROUGHAO;

	Material GuardianMaterial;
	GuardianMaterial.AddTexture(GuardianAlbedo);
	GuardianMaterial.AddTexture(GuardianNormal);
	GuardianMaterial.AddTexture(GuardianMetRoughAO);

	// GUARDIAN
	IEntity* GuardianEntity = new Entity;
	Chroma::Scene::AddEntity(GuardianEntity);
	MeshComponent* GuardianMeshComponent = new StaticMesh("resources/forestClearing/models/guardian.fbx");
	GuardianMeshComponent->SetMaterial(GuardianMaterial);
	GuardianEntity->AddComponent(GuardianMeshComponent);


	// TERRAIN
	// ____________________________________________________

	Texture groundAlbedo("resources/forestClearing/textures/terrain/albedo.jpg");
	groundAlbedo.m_Type = Texture::ALBEDO;
	Texture groundNormal("resources/forestClearing/textures/terrain/normal.jpg");
	groundNormal.m_Type = Texture::NORMAL;
	Texture groundMetRoughAO("resources/forestClearing/textures/terrain/metRoughAO.jpg");
	groundMetRoughAO.m_Type = Texture::METROUGHAO;

	Material groundMat;
	groundMat.AddTexture(groundAlbedo);
	groundMat.AddTexture(groundNormal);
	//groundMat.AddTexture(flatNormal);
	groundMat.AddTexture(groundMetRoughAO);
	groundMat.SetUVMultiply(2.0);

	IEntity* TerrainEntity = new Entity;
	Chroma::Scene::AddEntity(TerrainEntity);
	MeshComponent* TerrainMeshComponent = new StaticMesh("resources/forestClearing/models/terrain.fbx");
	TerrainMeshComponent->SetMaterial(groundMat);
	TerrainEntity->AddComponent(TerrainMeshComponent);
	//TerrainEntity->SetScale(glm::vec3(10.0, 1.0, 10.0));

	// ____________________________________________________


	// SCENE BUILD

	Chroma::Scene::GetRenderCamera()->SetPosition(glm::vec3(1.0, 2.0, 1.0));

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



		if (Chroma::Input::IsPressed(Chroma::Input::W))
		{
			for (const UID& meshComp : Chroma::Scene::GetMeshComponentUIDs())
			{
				static_cast<MeshComponent*>(Chroma::Scene::GetComponent(meshComp))->SetCastsShadows(true);
			}
		}

		if (Chroma::Input::IsPressed(Chroma::Input::S))
		{
			for (const UID& meshComp : Chroma::Scene::GetMeshComponentUIDs())
			{
				static_cast<MeshComponent*>(Chroma::Scene::GetComponent(meshComp))->SetCastsShadows(false);
			}
		}
		// Vegetation Uniforms
		foliageShader.Use();
		foliageShader.SetUniform("windFrequency", glm::vec2(0.005));
		foliageShader.SetUniform("windDirection", Chroma::Scene::GetSunLight()->GetDirection());
		foliageShader.SetUniform("windSpeed", 0.05f);
		foliageShader.SetUniform("windStrength", 0.5f);
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