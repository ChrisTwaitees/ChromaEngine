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
#include "../screenmanager/ChromaScreenManagerConfig.h"
#include "../shaders/Shader.h"
#include "../lights/Light.h"

class ShadowBuffer
{
private:
	unsigned int depthMapFBO;
	unsigned int resolutionFactor{ 2 };
	// resolution
	unsigned int width{ SHADOW_WIDTH / resolutionFactor };
	unsigned int height{ SHADOW_HEIGHT / resolutionFactor };
	// shaders
	// depth shader
	std::string depthVtxSource = "resources/shaders/vertexDepthMap.glsl";
	std::string depthFragSource = "resources/shaders/fragDepthMap.glsl";
	Shader depthShader;
	// setup 
	void initialize();
	// CALC LIGHTS
	glm::mat4 lightSpaceMatrix;
public:
	// getters and setters
	unsigned int GetShadowMapWidth() { return width; };
	unsigned int GetShadowMapHeight() { return height; };
	// shadow depth buffer
	unsigned int texShadowMap;
	void bind();
	
	// calculate shadows
	void calculateShadows(const Light& light);

	// constructors
	ShadowBuffer();
	~ShadowBuffer();
};

#endif

