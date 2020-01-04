#ifndef _CHROMA_SSAO_BUFFER_
#define _CHROMA_SSAO_BUFFER_

#include <vector>
#include <random>
#include <memory>

#include <glm/glm.hpp>

#include <buffer/Framebuffer.h>
#include <scene/Scene.h>


class SSAOBuffer : public Framebuffer
{
	// random generators
	std::uniform_real_distribution<float> randomFloats{ 0.0, 1.0 };
	std::default_random_engine generator;

	// shader
	const char* fragSource{ "resources/shaders/fragSSAO.glsl" };
	const char* fragBlurSource{ "resources/shaders/fragSSAOBlur.glsl" };
	const char* vtxSource{ "resources/shaders/frameBufferVertex.glsl" };
	Shader SSAOShader{ fragSource , vtxSource };
	Shader SSAOBlurShader{ fragBlurSource, vtxSource };

	// buffers
	unsigned int ssaoFBO, ssaoColorBuffer;
	unsigned int ssaoBlurFBO, ssaoColorBufferBlur;
	void generateBuffers();

	// initialize
	void Initialize() override;

	// sampleKernels
	unsigned int kernelSamples{ 16 };
	std::vector<glm::vec3> ssaoKernel;
	void generateSampleKernel();

	// noise texture
	float noiseSize{ 16.0f };
	unsigned int noiseTexture;
	std::vector<glm::vec3> ssaoNoise;
	glm::vec2 noiseScale;
	void generateNoiseTexture();

	// functions
	void sendKernelSamplesToShader();
	void configureShaders();


public:
	void Draw(unsigned int& gViewPosition, unsigned int& gNormal);
	virtual unsigned int getTexture() override { return ssaoColorBufferBlur; };

	SSAOBuffer();
	~SSAOBuffer();
};

#endif