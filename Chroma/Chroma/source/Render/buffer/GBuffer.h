#ifndef _CHROMA_GBUFFER_
#define _CHROMA_GBUFFER_

#include <buffer/Framebuffer.h>
#include <buffer/PostFXBuffer.h>
#include <buffer/ShadowBuffer.h>
#include <buffer/SSAOBuffer.h>
#include <ibl/IBL.h>
#include <scene/ChromaScene.h>


class GBuffer : public Framebuffer
{
	// shaders
	const char* fragLightingPass{ "resources/shaders/fragGBufferLit.glsl" };
	const char* vtxLightingSoure{ "resources/shaders/frameBufferVertex.glsl" };
	const char* fragGeometryPass{ "resources/shaders/fragGBufferGeometry.glsl" };
	const char* vtxGeometrySource{ "resources/shaders/vertexGBufferLit.glsl" };
	Shader m_geometryPassShader{ fragGeometryPass, vtxGeometrySource };
	Shader m_lightingPassShader{ fragLightingPass, vtxLightingSoure };

	// buffers
	unsigned int m_gBuffer;
	unsigned int m_gRBO;
	Framebuffer* m_SSAOBuffer{ new SSAOBuffer };

	// scene
	ChromaScene* m_scene;
	ShadowBuffer* mShadowbuffer;
	Framebuffer* m_postFXBuffer;

	// functions
	void initialize() override;
	void bindAllGBufferTextures();

	// gbuffer textures
	unsigned int gPosition, gViewPosition, gViewNormal, gFragPosLightSpace;
	unsigned int gAlbedo, gNormal, gMetRoughAO;

	// passes
	void calculateShadows();
	void drawGeometryPass();
	void drawLightingPass();
	void blitDepthBuffer();

	// uniforms
	void configureShaders();
	void setLightingUniforms();
	void updateTransformUniforms() override;

public:
	// functions
	void Bind() override;
	void Draw() override;
	// structors
	GBuffer(ChromaScene*& m_scene, Framebuffer*& m_postFXBuffer);
	~GBuffer();
};

#endif