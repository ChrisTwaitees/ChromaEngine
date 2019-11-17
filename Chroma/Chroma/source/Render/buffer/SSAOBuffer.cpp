#include "SSAOBuffer.h"

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

void SSAOBuffer::generateBuffers()
{
	// generate ssaoFBO Framebuffer Object
	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	// generate ssao ColorBuffer Texture Object
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

	// blur buffers
	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	// generate and bind blur texture
	glGenTextures(1, &ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
}

void SSAOBuffer::initialize()
{
	setupQuad();
	generateSampleKernel();
	generateNoiseTexture();
	generateBuffers();
	configureShaders();
}

void SSAOBuffer::generateSampleKernel()
{
	// hemisphere kernel based on a biased distance function
	// https://learnopengl.com/Advanced-Lighting/SSAO
	for (unsigned int i = 0; i < kernelSamples; i++)
	{
		glm::vec3 sample(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator)
		);
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / float(kernelSamples);
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}
}

void SSAOBuffer::generateNoiseTexture()
{
	// generate random noise tile
	for (unsigned int i = 0; i < noiseSize; i++)
	{
		glm::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
		ssaoNoise.push_back(noise);
	}
	// set noise scale for uniform
	noiseScale = glm::vec2(float(SCREEN_WIDTH) / std::sqrtf(noiseSize), float(SCREEN_HEIGHT) / std::sqrtf(noiseSize));
	// generating texture
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// creating tileable texture with GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void SSAOBuffer::sendKernelSamplesToShader()
{
	for (unsigned int i = 0; i < kernelSamples; i++)
		SSAOShader.setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
}

void SSAOBuffer::configureShaders()
{
	SSAOShader.use();
	SSAOShader.setInt("kernelSize", kernelSamples);
	SSAOShader.setFloat("radius", 0.5f);
	SSAOShader.setFloat("bias", 0.025f);
	SSAOShader.setVec2("noiseScale", noiseScale);

	SSAOShader.setInt("gViewPosition", 0);
	SSAOShader.setInt("gViewNormal", 1);
	SSAOShader.setInt("texNoise", 2);

	SSAOShader.setVec2("scale", scale);
	SSAOShader.setVec2("offset", offset);

	SSAOBlurShader.use();
	SSAOBlurShader.setInt("ssaoInput", 0);

	SSAOBlurShader.setVec2("scale", scale);
	SSAOBlurShader.setVec2("offset", offset);
}

void SSAOBuffer::Draw(unsigned int& gViewPosition, unsigned int& gNormal, ChromaScene*& scene)
{
	// binding frame buffer and clearing color buffer
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	// updating shader uniforms
	SSAOShader.use();
	sendKernelSamplesToShader();
	SSAOShader.setMat4("projection", scene->getRenderCamera()->getProjectionMat());
	//sending textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gViewPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	renderQuad();
	unBind();

	// binding frame buffer and clearing color buffer
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	// updating shader uniforms
	SSAOBlurShader.use();
	// sending textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	renderQuad();
	unBind();

}

SSAOBuffer::SSAOBuffer()
{
	initialize();
}


SSAOBuffer::~SSAOBuffer()
{
}
