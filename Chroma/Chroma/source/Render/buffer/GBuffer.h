#ifndef _CHROMA_GBUFFER_
#define _CHROMA_GBUFFER_


#include <buffer/IFramebuffer.h>
#include <buffer/PostFXBuffer.h>
#include <shadow/ShadowBuffer.h>
#include <buffer/SSAOBuffer.h>
#include <ibl/IBL.h>
#include <scene/Scene.h>
#include <model/Model.h>


namespace Chroma
{

	class GBuffer : public IFramebuffer
	{
	public:
		// functions
		void Draw() override;
		void ResizeBuffers() override;

		inline unsigned int GetSSAOTexture() const { return m_SSAOBuffer->GetTexture(); }

		// structors
		GBuffer(IFramebuffer*& m_PostFXBuffer);
		~GBuffer();

	private:
		// temp
		unsigned int m_LightingFrameBuffer{0};

		// shaders
		const char* fragLightingPass{ "resources/shaders/fragGBufferLit.glsl" };
		const char* vtxLightingSoure{ "resources/shaders/frameBufferVertex.glsl" };
		Shader m_lightingPassShader{ fragLightingPass, vtxLightingSoure };

		const char* fragGeometryPass{ "resources/shaders/fragGBufferGeometry.glsl" };
		const char* vtxGeometrySource{ "resources/shaders/vertexGBufferLit.glsl" };
		Shader m_geometryPassShader{ fragGeometryPass, vtxGeometrySource };


		// buffers
		IFramebuffer* m_SSAOBuffer{ new SSAOBuffer };

		// scene
		IFramebuffer* m_PostFXBuffer;

		// functions
		void Init() override;
		void BindGBufferTextures();

		// gbuffer textures
		unsigned int gPosition, gViewPosition, gViewNormal, gFragPosLightSpace;
		unsigned int gAlbedo, gNormal, gMetRoughAO;
		unsigned int gDepth;

		// passes
		void DrawGeometryPass();
		void DrawLightingPass();
		void BlitDepthBuffer();

		// uniforms
		void ConfigureShaders();
		void SetTransformUniforms() override;


	};
}

#endif