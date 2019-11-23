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
	std::vector<Light*> Lights;

	// point light positions
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  2.2f,  2.0f),
		glm::vec3(2.3f, -1.3f, -4.0f),
	};
	// dancing point lights
	for (glm::vec3 pos : pointLightPositions)
	{
		Light* pointLight = new Light(pos, Light::POINT);
		pointLight->setIntensity(3.0f);
		pointLight->quadratic *= 4.0f;
		pointLight->linear *= 2.0f;
		Lights.push_back(pointLight);
	}
	// SUNLIGHT
	Light* Sun = new Light(Light::SUNLIGHT, glm::vec3(0.2, -0.8, 0.3), 0.8f);
	Lights.push_back(Sun);


	// SHADERS
	Shader litNormalsShader("resources/shaders/fragLitShadowsNormals.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
	Shader litShader("resources/shaders/fragLitShadows.glsl", "resources/shaders/vertexLitShadows.glsl");
	Shader refractionShader("resources/shaders/fragRefraction.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader constantShader("resources/shaders/fragConstant.glsl", "resources/shaders/vertexShaderLighting.glsl");
	Shader alphaShader("resources/shaders/fragAlpha.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
	Shader debugNormalsShader("resources/shaders/fragDebugNormals.glsl", "resources/shaders/vertexDebugNormals.glsl", "resources/shaders/geometryDebugNormals.glsl");


	// TEXTURES
	Texture blackAlbedo("resources/textures/black.jpg");
	Texture greyAlbedo("resources/textures/grey.jpg");
	Texture whiteAlbedo("resources/textures/white.jpg");

	Texture sandyNormal("resources/textures/sandy_normal.jpg");
	sandyNormal.type = Texture::NORMAL;


	// TERRAIN
	IChromaEntity* TerrainEntity = new ChromaEntity;
	Scene->addEntity(TerrainEntity);
	ChromaMeshComponent* TerrainMeshComponent = new Terrain;
	TerrainMeshComponent->bindTexture(greyAlbedo);
	TerrainMeshComponent->bindTexture(sandyNormal);
	TerrainMeshComponent->bindShader(&litNormalsShader);
	TerrainEntity->addComponent(TerrainMeshComponent);

	// SPHERES
	// Sphere Positions
	glm::vec3 spherePositions[] = {
		glm::vec3(-5.f,  1.0f,  0.0f),
		glm::vec3(-2.5f,  1.0f,  0.0f),
		glm::vec3(0.f,  1.0f,  0.0f),
		glm::vec3(2.5f,  1.0f,  0.0f),
		glm::vec3(5.0f,  1.0f,  0.0f)
	};
	
	for (glm::vec3 position : spherePositions)
	{
		IChromaEntity* SphereEntity = new ChromaEntity;
		Scene->addEntity(SphereEntity);
		SphereEntity->setName("Sphere");
		ChromaMeshComponent* SphereMeshComponent = new SpherePrimitive;
		ChromaPhysicsComponent* SphereRigidComponent = new ChromaPhysicsComponent();
		SphereRigidComponent->setCollisionShape(ColliderShape::Convex);
		SphereRigidComponent->setCollisionState(ColliderState::Kinematic);
		SphereMeshComponent->bindTexture(sandyNormal);
		SphereMeshComponent->bindTexture(greyAlbedo);
		SphereMeshComponent->bindShader(&litNormalsShader);
		SphereEntity->addComponent(SphereMeshComponent);
		SphereEntity->addComponent(SphereRigidComponent);
		SphereEntity->setPosition(position);
	}




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

		// Updating sun
		Sun->setPosition(Sun->getDirection() * -20.0f);
		//Sunlight Rotation
		Sun->setDirection(glm::normalize((glm::vec3(std::sin(GameTime * 1.0f), -glm::abs(std::sin(GameTime * 1.0f)), -std::cos(GameTime * 1.0f)))));

		for (Light* light : Lights)
			light->setDiffuse(glm::vec3(1.0));


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