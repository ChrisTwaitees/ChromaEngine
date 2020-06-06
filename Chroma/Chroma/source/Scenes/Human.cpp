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



int main3()
{
	// INIT CHROMA
	Chroma::Engine::Init();

	// Initialize Scene
	Chroma::Scene::PreSceneBuild();

	// POPULATE SCENE CONTENTS
	// ------------------------------------------------------------------------------------------
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
		Light* pointLight = new Light(pos, Chroma::Type::Light::kPointLight);
		Lights.push_back(pointLight);
	}

	// SUNLIGHT
	Light* Sun = new Light(Chroma::Type::Light::kSunlight, glm::vec3(0.2, -0.8, 0.3), 2.0f);
	Sun->SetDiffuse(glm::vec3(1.0));
	Sun->SetIntensity(3.0);
	Lights.push_back(Sun);
	Chroma::Scene::SetLights(Lights);

	// ____________________________________________________
	// ENTITIES
	// ____________________________________________________

	IEntity* HumanEntity = new Entity;
	HumanEntity->SetName("Human Model");
	Chroma::Scene::AddEntity(HumanEntity);
	HumanEntity->SetScale(glm::vec3(40.0f));
	HumanEntity->SetTranslation(glm::vec3(-5, 0, 0));

	IEntity* HumanEntity2 = new Entity;
	HumanEntity2->SetName("Human Model2");
	Chroma::Scene::AddEntity(HumanEntity2);
	HumanEntity2->SetScale(glm::vec3(40.0f));
	HumanEntity2->SetTranslation(glm::vec3(5, 0, 0));

	// ____________________________________________________
	// SHADERS
	// ____________________________________________________

	Shader SemiTransparentShader("resources/shaders/fragPBRAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
	Shader PBRSkinShaderExperimental("resources/shaders/fragSSSS.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
	Shader PBRSkinShader("resources/shaders/fragSSSS_backup.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");

	// ____________________________________________________
	// TEXTURES
	// ____________________________________________________

	//// Default
	// Generic
	Texture greyAlbedo("resources/textures/colors/grey.jpg");
	Texture gridAlbedo("resources/animation/textures/grid.jpg");
	Texture flatNormal("resources/textures/test/flat_normal.jpg");

	//// Jacket
	//Texture jacketAlbedo("resources/human/textures/jacket/Jacket_Colour.jpg");
	//jacketAlbedo.m_Type = Texture::ALBEDO;
	//Texture jacketNormal = Chroma::ResourceManager::LoadTexture("resources/human/textures/jacket/Jacket_Normal.jpg");
	//jacketNormal.m_Type = Texture::NORMAL;
	//Texture jacketMetRoughAO = Chroma::ResourceManager::LoadTexture("resources/human/textures/jacket/MetRoughAO.jpg");
	//jacketMetRoughAO.m_Type = Texture::METROUGHAO;

	// Head
	Texture headAlbedo = Chroma::ResourceManager::LoadTexture("resources/human/textures/head/head_albedo.jpg");
	headAlbedo.m_Type = Chroma::Type::Texture::kAlbedo;
	Texture headNormal = Chroma::ResourceManager::LoadTexture("resources/human/textures/head/head_normal.jpg");
	headNormal.m_Type = Chroma::Type::Texture::kNormal;
	Texture headMetRoughAO = Chroma::ResourceManager::LoadTexture("resources/human/textures/head/head_metroughao.jpg");
	headMetRoughAO.m_Type = Chroma::Type::Texture::kMetRoughAO;
	Texture headTranslucency = Chroma::ResourceManager::LoadTexture("resources/human/textures/head/head_translucency.jpg");
	headTranslucency.m_Type = Chroma::Type::Texture::kTranslucency;



	// ____________________________________________________
	// MODELS
	// ____________________________________________________

	////Jacket
	//MeshComponent* JacketMeshComponent = new Model("resources/human/Jacket/Jacket.fbx");
	//JacketMeshComponent->SetShader(PBRShader);
	//JacketMeshComponent->AddTexture(jacketAlbedo);
	//JacketMeshComponent->AddTexture(jacketNormal);
	//JacketMeshComponent->AddTexture(jacketMetRoughAO);
	//HumanEntity->AddComponent(JacketMeshComponent);

	// Head
	MeshComponent* HeadMeshComponent = new Model("resources/human/Head/Head.fbx");
	HeadMeshComponent->SetIsForwardLit(true);
	HeadMeshComponent->SetIsLit(false);
	HeadMeshComponent->SetShader(PBRSkinShader);
	HeadMeshComponent->AddTexture(headAlbedo);
	HeadMeshComponent->AddTexture(headNormal);
	HeadMeshComponent->AddTexture(headMetRoughAO);
	HeadMeshComponent->AddTexture(headTranslucency);
	HumanEntity->AddComponent(HeadMeshComponent);


	// Head
	MeshComponent* HeadMeshComponent2 = new Model("resources/human/Head/Head.fbx");
	HeadMeshComponent2->SetIsForwardLit(true);
	HeadMeshComponent2->SetIsLit(false);
	HeadMeshComponent2->SetShader(PBRSkinShaderExperimental);
	HeadMeshComponent2->AddTexture(headAlbedo);
	HeadMeshComponent2->AddTexture(headNormal);
	HeadMeshComponent2->AddTexture(headMetRoughAO);
	HeadMeshComponent2->AddTexture(headTranslucency);
	HumanEntity2->AddComponent(HeadMeshComponent2);

	// TERRAIN
	// ____________________________________________________
	IEntity* TerrainEntity = new Entity;
	Chroma::Scene::AddEntity(TerrainEntity);
	MeshComponent* TerrainMeshComponent = new Terrain;
	TerrainMeshComponent->SetShader(PBRSkinShaderExperimental);
	TerrainMeshComponent->AddTexture(gridAlbedo);
	TerrainMeshComponent->AddTexture(flatNormal);
	TerrainEntity->AddComponent(TerrainMeshComponent);
	TerrainEntity->SetScale(glm::vec3(10.0, 1.0, 10.0));
	// ____________________________________________________


	// Eyelashes 
	MeshComponent* EyelashesMeshComponent = new Model("resources/human/Head/Eyelashes.fbx");
	HumanEntity->AddComponent(EyelashesMeshComponent);

	// Eyebrows
	MeshComponent* EyebrowsMeshComponent = new Model("resources/human/Head/Eyebrows.fbx");
	HumanEntity->AddComponent(EyebrowsMeshComponent);


	Chroma::Scene::PostSceneBuild();

	



	// RENDER LOOP
	// -----------
	while (Chroma::Screen::IsRunning())
	{


		//Sunlight Rotation	
		Chroma::Scene::GetSunLight()->SetTranslation(glm::vec3(std::sin(GAMETIME * SUNLIGHT_SPIN_SPEED) * SUNLIGHT_DISTANCE, SUNLIGHT_DISTANCE, std::cos(GAMETIME * SUNLIGHT_SPIN_SPEED) * SUNLIGHT_DISTANCE));
		Sun->SetDirection(-normalize(Sun->GetPosition()));

		// GAME TICK
		Chroma::Engine::Tick();

	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	Chroma::Screen::Close();
	return 0;
}

#endif