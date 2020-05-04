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
	unsigned int GetNumCascadeSplits() { return m_NumCascadeSplits; };
	void SetNumCascadeSplits(unsigned int const& newNumSplits) { m_NumCascadeSplits = newNumSplits; BuildCSMTextureArray(); };

	// getters and setters
	glm::mat4 GetLightSpaceMatrix() { return m_LightSpaceMatrix; };
	glm::mat4 GetCascadeLightSpaceMatrices() { return m_LightSpaceMatrix; };
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
	unsigned int m_NumCascadeSplits;
	unsigned int m_CascadedTexureArray;
	void BuildCSMTextureArray();
	// Dimensions
	std::vector<glm::mat4> m_CascadeLightSpaceMatrices;
	void CalcCascadeLightSpaceMatrices();
	std::vector<float> m_CascadeSplitDistances;
	float m_CascadeSplitDistanceRatio{0.5f};
	glm::mat4 m_LightSpaceMatrix;
	glm::mat4 m_LightOrthoMatrix;
	void CalcCascadeSplitDistances();

	void calcLightSpaceMatrix();
	// shaders
	std::string depthVtxSource = "resources/shaders/vertexDepthMap.glsl";
	std::string depthFragSource = "resources/shaders/fragEmpty.glsl";
	Shader m_DepthShader{ depthFragSource , depthVtxSource };
	// setup 
	void Initialize();

};

#endif

