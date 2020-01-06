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
#include <buffer/IFramebuffer.h>
#include <scene/Scene.h>

class ShadowBuffer : IFramebuffer
{
private:
	// DepthBuffers
	unsigned int depthMapFBO;
	// resolution
	unsigned int width{ SHADOW_WIDTH};
	unsigned int height{ SHADOW_HEIGHT};
	// shaders
	std::string depthVtxSource = "resources/shaders/vertexDepthMap.glsl";
	std::string depthFragSource = "resources/shaders/fragEmpty.glsl";
	Shader depthShader{ depthFragSource , depthVtxSource };
	// textures
	Texture ShadowMapTexture;
	// render
	void calcLightSpaceMatrix();
	// setup 
	void Initialize();
	void bindShadowMapToBuffer();
	glm::mat4 lightSpaceMatrix;

public:
	// getters and setters
	glm::mat4 getLightSpaceMatrix() { return lightSpaceMatrix; };
	unsigned int GetTexture() override { return ShadowMapTexture.ID; }

	// calculate shadows
	void DrawShadowMaps();

	// constructors
	ShadowBuffer();
	~ShadowBuffer();
};

#endif

