#ifndef _CHROMA_GBUFFER_
#define _CHROMA_GBUFFER_

#include "buffer/Framebuffer.h"
#include "buffer/PostFXBuffer.h"
#include "buffer/ShadowBuffer.h"
#include "buffer/SSAOBuffer.h"



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
	const ChromaSceneManager* mScene;
	ShadowBuffer* mShadowbuffer;
	Framebuffer* mPostFXBuffer;

	// functions
	void initialize() override;
	void bindAllGBufferTextures();

	// GBUFFER TEXTURES
	unsigned int gNormal, gPosition, gAlbedoRoughness, gMetalnessSpecular, gFragPosLightSpace, gViewPosition, gViewNormal;

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

	GBuffer(const ChromaSceneManager* mScene, Framebuffer*& mPostFXBuffer);
	~GBuffer();
};

#endif