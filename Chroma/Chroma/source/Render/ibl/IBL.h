#ifndef _CHROMA_IBL_
#define _CHROMA_IBL_
// stl
#include <string>
// chroma
#include <model/BoxPrimitive.h>
#include <texture/HDRTexture.h>
#include <shader/Shader.h>
#include <screenManager/ChromaScreenManagerConfig.h>

class IBL
{
	// hdr texture
	HDRTexture m_HDRtexture{ "resources/textures/ibl/newportloft_ibl/newportloft.hdr" };
	// capture cube
	BoxPrimitive m_captureCube;
	// capture cube shader
	Shader m_captureCubeShader{ "resources/shaders/fragIBL.glsl", "resources/shaders/vertexCubeMap.glsl" };
	// capture buffers
	unsigned int m_captureFBO, m_captureRBO;
	// environment buffer texture
	unsigned int m_envCubeMap;

	void initialize();
	void initCaptureBuffer();
	void initEnvCubeMap();
	void captureEnvCubeMap();
public:

	void Draw();
	unsigned int getEnvCubeMapID() { return m_envCubeMap; };
	unsigned int getHDRTextureID() { return m_HDRtexture.ID; };

	template <typename UniformType>
	void setUniform(std::string uniformName, UniformType uniform)
	{
		m_captureCubeShader.use();
		m_captureCubeShader.setUniform(uniformName, uniform);
	}
	IBL();
	~IBL();
};

#endif