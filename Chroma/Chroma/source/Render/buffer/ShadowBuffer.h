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
#include "buffer/Framebuffer.h"

class ShadowBuffer : Framebuffer
{
private:
	// Chroma Scene
	ChromaScene* mScene;
	// DepthBuffers
	unsigned int depthMapFBO;
	// resolution
	unsigned int width{ SHADOW_WIDTH};
	unsigned int height{ SHADOW_HEIGHT};
	// shaders
	std::string depthVtxSource = "resources/shaders/vertexDepthMap.glsl";
	std::string depthFragSource = "resources/shaders/fragDepthMap.glsl";
	Shader depthShader{ depthFragSource , depthVtxSource };
	// textures
	Texture ShadowMapTexture;
	// render
	void calcLightSpaceMatrix();
	// setup 
	void initialize();
	void bindShadowMapToBuffer();
	glm::mat4 lightSpaceMatrix;

public:
	// getters and setters
	glm::mat4 getLightSpaceMatrix() { return lightSpaceMatrix; };
	unsigned int getTexture() override { return ShadowMapTexture.ID; }

	// calculate shadows
	void calculateShadows();

	// constructors
	ShadowBuffer(ChromaScene*& Scene);
	~ShadowBuffer();
};

#endif

