#ifndef _CHROMA_GBUFFER_
#define _CHROMA_GBUFFER_

#include <buffer/Framebuffer.h>
#include <buffer/PostFXBuffer.h>
#include <buffer/ShadowBuffer.h>
#include <buffer/SSAOBuffer.h>
#include <scene/ChromaScene.h>


class GBuffer : public Framebuffer
{
protected: 
	// shaders
	const char* fragLightingPass{ "resources/shaders/fragGBufferLit.glsl" };
	const char* vtxLightingSoure{ "resources/shaders/frameBufferVertex.glsl" };
	const char* fragGeometryPass{ "resources/shaders/fragGBufferGeometry.glsl" };
	const char* vtxGeometrySource{ "resources/shaders/vertexGBufferLit.glsl" };
	Shader geometryPassShader{ fragGeometryPass, vtxGeometrySource };
	Shader lightingPassShader{ fragLightingPass, vtxLightingSoure };

	// buffers
	unsigned int gBuffer;
	unsigned int gRBO;
	Framebuffer* mSSAOBuffer{ new SSAOBuffer };

	// scene
	ChromaScene* m_scene;
	ShadowBuffer* mShadowbuffer;
	Framebuffer* m_postFXBuffer;

	// functions
	void initialize() override;
	void bindAllGBufferTextures();

	// GBUFFER TEXTURES
	unsigned int gPosition, gViewPosition, gViewNormal, gFragPosLightSpace;
	unsigned int gAlbedo, gNormal, gMetRoughAO;

	// Passes
	void calculateShadows();
	void drawGeometryPass();
	void drawLightingPass();
	void blitDepthBuffer();

	// uniforms
	void configureShaders();
	void setLightingUniforms();
	void updateTransformUniforms() override;

public:

	void Bind() override;
	void Draw() override;

	GBuffer(ChromaScene*& m_scene, Framebuffer*& m_postFXBuffer);
	~GBuffer();
};

#endif