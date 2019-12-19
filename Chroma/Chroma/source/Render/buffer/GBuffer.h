#ifndef _CHROMA_GBUFFER_
#define _CHROMA_GBUFFER_

#include <buffer/Framebuffer.h>
#include <buffer/PostFXBuffer.h>
#include <buffer/ShadowBuffer.h>
#include <buffer/SSAOBuffer.h>
#include <ibl/IBL.h>
#include <scene/ChromaScene.h>
#include <model/Model.h>


class GBuffer : public Framebuffer
{
	// shaders
	const char* fragLightingPass{ "resources/shaders/fragGBufferLit.glsl" };
	const char* vtxLightingSoure{ "resources/shaders/frameBufferVertex.glsl" };
	Shader m_lightingPassShader{ fragLightingPass, vtxLightingSoure };

	const char* fragGeometryPass{ "resources/shaders/fragGBufferGeometry.glsl" };
	const char* vtxGeometrySource{ "resources/shaders/vertexGBufferLit.glsl" };
	Shader m_geometryPassShader{ fragGeometryPass, vtxGeometrySource };


	// buffers
	unsigned int m_gBuffer;
	unsigned int m_gRBO;
	Framebuffer* m_SSAOBuffer{ new SSAOBuffer };

	// scene
	ChromaScene* m_Scene;
	ShadowBuffer* mShadowbuffer;
	Framebuffer* m_postFXBuffer;

	// functions
	void Initialize() override;
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

	// cubemap TEST
	CubeMap testCubeMap{ "resources/textures/skybox/blueskywater"};

public:
	// functions
	void Bind() override;
	void Draw() override;
	// structors
	GBuffer(ChromaScene*& m_Scene, Framebuffer*& m_postFXBuffer);
	~GBuffer();
};

#endif