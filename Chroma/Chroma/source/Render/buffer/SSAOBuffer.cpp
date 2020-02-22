#include "SSAOBuffer.h"

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

void SSAOBuffer::generateBuffers()
{
	// generate ssaoFBO IFramebuffer Object
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

void SSAOBuffer::Initialize()
{
	generateSampleKernel();
	generateNoiseTexture();
	generateBuffers();
	ConfigureShaders();
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
	// set noise m_Scale for uniform
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
		SSAOShader.SetVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
}

void SSAOBuffer::ConfigureShaders()
{
	SSAOShader.Use();
	SSAOShader.SetInt("kernelSize", kernelSamples);
	SSAOShader.SetFloat("radius", 0.5f);
	SSAOShader.SetFloat("bias", 0.025f);
	SSAOShader.SetVec2("noiseScale", noiseScale);

	SSAOShader.SetInt("gViewPosition", 0);
	SSAOShader.SetInt("gViewNormal", 1);
	SSAOShader.SetInt("texNoise", 2);

	SSAOShader.SetVec2("scale", m_Scale);
	SSAOShader.SetVec2("offset", m_Offset);

	SSAOBlurShader.Use();
	SSAOBlurShader.SetInt("ssaoInput", 0);

	SSAOBlurShader.SetVec2("scale", m_Scale);
	SSAOBlurShader.SetVec2("offset", m_Offset);
}

void SSAOBuffer::Draw(unsigned int& gViewPosition, unsigned int& gNormal)
{
	// binding frame buffer and clearing color buffer
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	// updating shader uniforms
	SSAOShader.Use();
	sendKernelSamplesToShader();
	SSAOShader.SetMat4("projection", Chroma::Scene::GetRenderCamera()->GetProjectionMatrix());
	//sending textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gViewPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	RenderQuad();
	UnBind();

	// binding frame buffer and clearing color buffer
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	// updating shader uniforms
	SSAOBlurShader.Use();
	// sending textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	RenderQuad();
	UnBind();

}

void SSAOBuffer::ResizeBuffers()
{
	// textures
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL);
}


SSAOBuffer::SSAOBuffer()
{
	Initialize();
}


SSAOBuffer::~SSAOBuffer()
{
}
