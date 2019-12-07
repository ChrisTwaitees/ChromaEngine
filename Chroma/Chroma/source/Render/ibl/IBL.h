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
	// shaders
	Shader m_envMapShader{ "resources/shaders/fragIBL.glsl", "resources/shaders/vertexCubeMap.glsl" };
	Shader m_irradienceMapShader{ "resources/shaders/fragIrradiance.glsl", "resources/shaders/vertexCubeMap.glsl" };
	// capture buffers
	unsigned int m_captureFBO, m_captureRBO;
	// environment buffer texture
	unsigned int m_envCubeMap, m_irradianceMap;

	// functions
	void initialize();
	void generateEnvCubeMap();
	void generateIrradianceMap();
public:

	void Draw();
	inline unsigned int getEnvCubeMapID() { return m_envCubeMap; };
	inline unsigned int getIrradianceMapID() { return m_irradianceMap; };
	inline unsigned int getHDRTextureID() { return m_HDRtexture.ID; };

	template <typename UniformType>
	void setUniform(std::string uniformName, UniformType uniform)
	{
		m_envMapShader.use();
		m_envMapShader.setUniform(uniformName, uniform);
	}
	IBL();
	~IBL();
};

#endif