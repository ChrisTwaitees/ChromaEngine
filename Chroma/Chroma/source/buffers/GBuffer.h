#ifndef _CHROMA_GBUFFER_
#define _CHROMA_GBUFFER_

#include "../buffers/Framebuffer.h"
#include "../buffers/ShadowBuffer.h"
#include "../scene/ChromaScene.h"


class GBuffer : public Framebuffer
{
protected: 
	const char* fragLightingPass{ "resources/shaders/fragGBufferLit.glsl" };
	const char* vtxLightingSoure{ "resources/shaders/frameBufferVertex.glsl" };
	const char* fragGeometryPass{ "resources/shaders/fragGBuffer.glsl" };
	const char* vtxGeometrySource{ "resources/shaders/vertexLitShadowsNormals.glsl" };

	unsigned int gBuffer;
	unsigned int gRBO;

	Shader geometryPassShader{ fragGeometryPass, vtxGeometrySource };
	Shader lightingPassShader{ fragLightingPass, vtxLightingSoure };
	const ChromaScene* mScene;
	ShadowBuffer* mShadowbuffer;

	void initialize() override;
	void updateTransformUniforms() override;
	void configureShaders();
	void bindAllGBufferTextures();
	void setLightingUniforms();

	// GBUFFER TEXTURES
	unsigned int gNormal, gPosition, gAlbedoRoughness, gMetalnessSpecular, gFragPosLightSpace;

public:

	void Bind() override;
	void Draw() override;

	GBuffer(const ChromaScene*& Scene, ShadowBuffer*& shadowbuffer);
	~GBuffer();
};

#endif