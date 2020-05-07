#ifndef _CHROMA_SHADOWBUFFER_
#define _CHROMA_SHADOWBUFFER_

// common
#include <common/PrecompiledHeader.h>

// Chroma
#include <buffer/IFramebuffer.h>
#include <scene/Scene.h>

class ShadowBuffer : public IFramebuffer
{
public:
	void ResizeBuffers() override;

	// Cascade Splits
	unsigned int GetNumCascadeSplits() { return m_NumCascadeSplits; };
	void SetNumCascadeSplits(unsigned int const& newNumSplits) { m_NumCascadeSplits = newNumSplits; BuildCSMTextureArray(); };

	// getters and setters
	glm::mat4 GetLightSpaceMatrix() { return m_LightSpaceMatrix; };
	unsigned int GetTexture() override { return 0; }

	// calculate shadows
	void DrawShadowMaps();

	// constructors
	ShadowBuffer();
	~ShadowBuffer();

private:
	// Buffer
	unsigned int m_CascadeShadowFBO;

	// Resolution
	unsigned int m_ShadowMapSize{ 2048 };

	// Textures
	void BuildCSMTextureArray();
	unsigned int m_NumCascadeSplits;
	unsigned int m_CascadedTexureArray;
	
	// Dimensions
	glm::mat4 m_LightSpaceMatrix;

	void CalculateCascadeLightSpaceMatrices();
	std::vector<glm::mat4> m_CascadeLightSpaceMatrices;

	void CalculateCascadeSplitDistances();
	float m_CascadeSplitDistanceRatio{0.5f};
	std::vector<float> m_CascadeSplitDistances;

	//float m_ShadowNear{ 0.0f }, m_ShadowFar{ 50.0f };
	// Depth Shader
	std::string depthVtxSource = "resources/shaders/vertexDepthMap.glsl";
	std::string depthFragSource = "resources/shaders/fragEmpty.glsl";
	Shader m_DepthShader{ depthFragSource , depthVtxSource };

	// Setup 
	void Initialize();

};

#endif

