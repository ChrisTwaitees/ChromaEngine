#ifndef _CHROMA_SHADOWBUFFER_
#define _CHROMA_SHADOWBUFFER_
// stl
#include <iostream>
#include <vector>
// glad
#include <glad/glad.h>
// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Chroma
#include "../renderer/ChromaRendererConfig.h"
#include "../scene/ChromaScene.h"

class ShadowBuffer
{
private:
	// Chroma Scene
	ChromaScene* mScene;
	// DepthBuffers
	unsigned int depthMapFBO;
	unsigned int resolutionFactor{ 2 };
	// resolution
	unsigned int width{ SHADOW_WIDTH / resolutionFactor };
	unsigned int height{ SHADOW_HEIGHT / resolutionFactor };
	// shaders
	std::string depthVtxSource = "resources/shaders/vertexDepthMap.glsl";
	std::string depthFragSource = "resources/shaders/fragDepthMap.glsl";
	Shader depthShader;
	// render
	void calcLightSpaceMatrix();
	// setup 
	void initialize();
	void bindShadowMapToBuffer();
	glm::mat4 lightSpaceMatrix;

public:
	// getters and setters
	unsigned int GetShadowMapWidth() { return width; };
	unsigned int GetShadowMapHeight() { return height; };
	glm::mat4 getLightSpaceMatrix() { return lightSpaceMatrix; };
	// shadow depth buffer
	Texture ShadowMapTexture;
	// calculate shadows
	void calculateShadows();
	// constructors
	ShadowBuffer(ChromaScene* Scene);
	~ShadowBuffer();
};

#endif

