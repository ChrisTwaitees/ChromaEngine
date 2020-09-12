#ifndef _CHROMA_HDR_BUFFER_
#define _CHROMA_HDR_BUFFER_

#include <buffer/IFramebuffer.h>

namespace Chroma
{
	class PostFXBuffer : public IFramebuffer
	{
	public:
		unsigned int GetTexture() override { return colorBuffersTextures[0]; }
		unsigned int GetFBO() override { return hdrFBO; };

		void Draw() override;
		void Draw(const bool& useBloom);
		void Bind() override;

		float m_BloomAmount{ 1.0f };
		float m_IndirectContribution{ 1.0f };
		float m_SSRContribution{ 1.0f };
		float m_VXIrradiance_Contribution{ 1.0f };
		float m_VXRradiance_Contribution{ 1.0f };
		float m_Exposure{ 1.0f };
		float m_Gamma{ 2.2f };

		PostFXBuffer();
		~PostFXBuffer();

	protected:
		// default HDR shader
		unsigned int hdrFBO;
		const char* vtxSource{ "resources/shaders/frameBufferVertex.glsl" };
		const char* fragSource{ "resources/shaders/fragBloomFrameBuffer.glsl" };
		// blur shader
		const char* blurfragSource{ "resources/shaders/fragBlur.glsl" };

		// Textures
		unsigned int colorBuffersTextures[2];

		// Bloom m_FBO and Textures
		void genBlurBuffer();
		unsigned int blurFBOs[2];
		unsigned int blurColorBuffers[2];

		// Blur
		Shader* blurShader;
		bool horizontal = true, first_iteration = true;
		int blurIterations{ 10 };
		void blurFragments();

		// Functions
		void Init() override;
		void SetTransformUniforms() override;
		void ConfigureShaders();
		void ResizeBuffers() override;
	};
}


#endif