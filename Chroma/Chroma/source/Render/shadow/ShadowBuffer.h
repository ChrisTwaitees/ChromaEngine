#ifndef _CHROMA_SHADOWBUFFER_
#define _CHROMA_SHADOWBUFFER_

// common
#include <common/PrecompiledHeader.h>

// Chroma
#include <buffer/IFramebuffer.h>
#include <scene/Scene.h>

class ShadowBuffer : IFramebuffer
{
public:
	void ResizeBuffers() override;

	// Cascade Splits
	unsigned int GetNumCascadeSplits() { return m_NumSplits; };
	void SetNumCascadeSplits(unsigned int const& newNumSplits) { m_NumSplits = newNumSplits; BuildCSMTextureArray(); };

	// getters and setters
	glm::mat4 GetLightSpaceMatrix() { return lightSpaceMatrix; };
	glm::mat4 GetCascadeLightSpaceMatrices() { return lightSpaceMatrix; };
	unsigned int GetTexture() override { return 0; }

	// calculate shadows
	void DrawShadowMaps();
	void BindShadowMaps();

	// constructors
	ShadowBuffer();
	~ShadowBuffer();

private:
	// resolution
	unsigned int m_ShadowMapSize{ 2048 };
	// Textures
	unsigned int m_NumSplits;
	unsigned int m_CascadedTexureArray;
	void BuildCSMTextureArray();
	// Dimensions
	std::vector<glm::mat4> m_CascadeLightSpaceMatrices;
	std::vector<float> m_CascadeSplitDistances;
	glm::mat4 lightSpaceMatrix;
	void CalcCascadeSplitDistances();

	void calcLightSpaceMatrix();
	// shaders
	std::string depthVtxSource = "resources/shaders/vertexDepthMap.glsl";
	std::string depthFragSource = "resources/shaders/fragEmpty.glsl";
	Shader depthShader{ depthFragSource , depthVtxSource };
	// setup 
	void Initialize();

};

#endif

