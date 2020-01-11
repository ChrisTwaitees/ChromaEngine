#ifndef _CHROMA_GBUFFER_
#define _CHROMA_GBUFFER_


#include <buffer/IFramebuffer.h>
#include <buffer/PostFXBuffer.h>
#include <buffer/ShadowBuffer.h>
#include <buffer/SSAOBuffer.h>
#include <ibl/IBL.h>
#include <scene/Scene.h>
#include <model/Model.h>
#include <core/Core.h>



class GBuffer : public IFramebuffer
{
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
	ShadowBuffer* m_Shadowbuffer;
	IFramebuffer* m_PostFXBuffer;

	// functions
	void Initialize() override;
	void BindGBufferTextures();

	// gbuffer textures
	unsigned int gPosition, gViewPosition, gViewNormal, gFragPosLightSpace;
	unsigned int gAlbedo, gNormal, gMetRoughAO;

	// passes
	void DrawShadowMaps();
	void DrawGeometryPass();
	void drawLightingPass();
	void BlitDepthBuffer();

	// uniforms
	void ConfigureShaders();
	void SetLightingUniforms();
	void UpdateTransformUniforms() override;

public:
	// functions
	void Draw() override;

	inline unsigned int GetPositionTexture() const { return gPosition; }
	inline unsigned int GetAlbedoTexture() const { return gAlbedo; }
	inline unsigned int GetNormalTexture() const { return gNormal; }
	inline unsigned int GetMetalRoughnessAO() const { return gMetRoughAO; }
	inline unsigned int GetSSAOTexture() const { return m_SSAOBuffer->GetTexture(); }
	inline unsigned int GetShadowBufferTexture() const { return m_Shadowbuffer->GetTexture(); }

	// structors
	GBuffer(IFramebuffer*& m_PostFXBuffer);
	~GBuffer();
};

#endif