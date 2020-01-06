#ifndef _CHROMA_IBL_
#define _CHROMA_IBL_
// stl
#include <string>
// chroma
#include <model/BoxPrimitive.h>
#include <model/NDCPlanePrimitive.h>
#include <texture/HDRTexture.h>
#include <shader/Shader.h>
#include <ChromaConfig.h>

class IBL
{

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[6] =	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};
	// hdr texture
	HDRTexture m_HDRtexture{ "resources/textures/ibl/newportloft_ibl/newportloft.hdr" };
	// capture primitives
	BoxPrimitive m_captureCube;
	NDCPlanePrimitive m_capturePlane;
	// shaders
	Shader m_envMapShader{ "resources/shaders/fragIBL.glsl", "resources/shaders/vertexCubeMap.glsl" };
	Shader m_irradienceMapShader{ "resources/shaders/fragIrradiance.glsl", "resources/shaders/vertexCubeMap.glsl" };
	Shader m_prefilterMapShader{ "resources/shaders/fragPrefilter.glsl", "resources/shaders/vertexCubeMap.glsl" };
	Shader m_brdfShader{ "resources/shaders/fragBRDF.glsl", "resources/shaders/frameBufferVertex.glsl" };
	// capture buffers
	unsigned int m_captureFBO, m_captureRBO;
	// textures
	unsigned int m_envCubeMap, m_irradianceMap, m_prefilterMap, m_brdfLUTTexture;

	// functions
	void Initialize();
	void generateEnvCubeMap();
	void generateIrradianceMap();
	void generatePrefilterMap();
	void generateBRDFLUTMap();
public:

	void Draw();

	void setIBLTexture(HDRTexture newHDRTexture);

	inline unsigned int getEnvCubeMapID() { return m_envCubeMap; };
	inline unsigned int GetIrradianceMapID() { return m_irradianceMap; };
	inline unsigned int GetPrefilterMapID() { return m_prefilterMap; };
	inline unsigned int getHDRTextureID() { return m_HDRtexture.ID; };
	inline unsigned int GetBRDFLUTID() { return m_brdfLUTTexture; };

	template <typename UniformType>
	void SetUniform(std::string uniformName, UniformType uniform)
	{
		m_envMapShader.use();
		m_envMapShader.SetUniform(uniformName, uniform);
	}
	IBL();
	~IBL() {};
};

#endif
