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
	unsigned int m_gBuffer;
	unsigned int m_gRBO;
	IFramebuffer* m_SSAOBuffer{ new SSAOBuffer };

	// scene
	ShadowBuffer* mShadowbuffer;
	IFramebuffer* m_PostFXBuffer;

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
	void BlitDepthBuffer();

	// uniforms
	void configureShaders();
	void setLightingUniforms();
	void UpdateTransformUniforms() override;

	// cubemap TEST
	CubeMap testCubeMap{ "resources/textures/skybox/blueskywater"};

	void RenderWithShader(IEntity* Entity);
public:
	// functions
	void Bind() override;
	void Draw() override;
	// structors
	GBuffer(IFramebuffer*& m_PostFXBuffer);
	~GBuffer();
};

#endif