#ifndef _CHROMA_SHADOWBUFFER_
#define _CHROMA_SHADOWBUFFER_

// common
#include <common/PrecompiledHeader.h>

// Chroma
#include <buffer/IFramebuffer.h>
#include <scene/Scene.h>

namespace Chroma
{
	class ShadowBuffer : public IFramebuffer
	{
	public:
		friend class Render;
		// getters and setters
		glm::mat4 GetLightSpaceMatrix() { return m_CascadeLightSpaceMatrices[0]; };
		unsigned int GetTexture() override { return m_CascadedTexureArray; }

		// calculate shadows
		void DrawShadowMaps();


		// constructors
		ShadowBuffer();
		~ShadowBuffer();

	private:
		// Buffer
		unsigned int m_CascadeShadowFBO;

		// Called from Render on CameraChanged
		void CalculateCascadeLightSpaceMatrices();

		// Resolution
		unsigned int m_ShadowMapSize{ 4096 };

		// Textures
		void BuildCSMTextureArray();
		unsigned int m_CascadedTexureArray;
	
		// Dimensions
		glm::mat4 m_LightSpaceMatrix;

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
		void Init();
		friend class VXGIBuffer;

	};
}

#endif

