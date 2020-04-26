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
public:
	// functions
	void Draw() override;
	void ResizeBuffers() override;

	void BindShadownMaps();
	glm::mat4 GetLightSpaceMatrix();

	inline unsigned int GetWSPositionTexture() const { return gPosition; }
	inline unsigned int GetVSPositionTexture() const { return gViewPosition; }
	inline unsigned int GetAlbedoTexture() const { return gAlbedo; }
	inline unsigned int GetWSNormalTexture() const { return gNormal; }
	inline unsigned int GetVSNormalTexture() const { return gViewNormal; }
	inline unsigned int GetMetalRoughnessAO() const { return gMetRoughAO; }
	inline unsigned int GetSSAOTexture() const { return m_SSAOBuffer->GetTexture(); }
	inline unsigned int GetShadowBufferTexture() const { return m_Shadowbuffer->GetTexture(); }
	inline unsigned int GetDepthTexture() const { return gDepth; }

	// structors
	GBuffer(IFramebuffer*& m_PostFXBuffer);
	~GBuffer();

private:
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
	unsigned int gDepth;

	// passes
	void DrawShadowMaps();
	void DrawGeometryPass();
	void DrawLightingPass();
	void BlitDepthBuffer();

	// uniforms
	void ConfigureShaders();
	void SetLightingUniforms();
	void UpdateTransformUniforms() override;


};

#endif