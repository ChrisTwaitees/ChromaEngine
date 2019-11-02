#ifndef _CHROMA_GBUFFER_
#define _CHROMA_GBUFFER_

#include "../buffers/Framebuffer.h"
#include "../buffers/ShadowBuffer.h"
#include "../buffers/SSAOBuffer.h"
#include "../scene/ChromaScene.h"


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
	Framebuffer* ssaoBuffer{ new SSAOBuffer };

	// scene
	const ChromaScene* mScene;
	ShadowBuffer* mShadowbuffer;

	// functions
	void initialize() override;
	void bindAllGBufferTextures();

	// GBUFFER TEXTURES
	unsigned int gNormal, gPosition, gAlbedoRoughness, gMetalnessSpecular, gFragPosLightSpace, gViewPosition, gViewNormal;

	// Passes
	void drawGeometryPass();
	void drawLightingPass();

	// uniforms
	void configureShaders();
	void setLightingUniforms();
	void updateTransformUniforms() override;

public:

	void Bind() override;
	void Draw() override;

	GBuffer(const ChromaScene*& Scene, ShadowBuffer*& shadowbuffer);
	~GBuffer();
};

#endif