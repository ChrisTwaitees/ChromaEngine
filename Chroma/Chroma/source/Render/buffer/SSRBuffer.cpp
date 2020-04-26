#include "SSRBuffer.h"
#include <render/Render.h>
#include <buffer/GBuffer.h>

void SSRBuffer::Draw()
{
	//Bind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_ScreenShader->Use();
	// Bind Textures
	// 0 VsNormals
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, static_cast<GBuffer*>(Chroma::Render::GetGBuffer())->GetVSNormalTexture());
	m_ScreenShader->SetUniform("vsNormals", 0);
	// 1 VsPositions
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, static_cast<GBuffer*>(Chroma::Render::GetGBuffer())->GetVSPositionTexture());
	m_ScreenShader->SetUniform("vsPositions", 1);
	// 2 testColor
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, static_cast<GBuffer*>(Chroma::Render::GetGBuffer())->GetAlbedoTexture());
	m_ScreenShader->SetUniform("vAlbedo", 2);
	// 3 Depth
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, static_cast<GBuffer*>(Chroma::Render::GetGBuffer())->GetDepthTexture());
	m_ScreenShader->SetUniform("vDepth", 3);

	// uniforms
	m_ScreenShader->SetUniform("vProjection", Chroma::Scene::GetRenderCamera()->GetProjectionMatrix());
	// using color attachment
	// setting transform uniforms
	UpdateTransformUniforms();
	RenderQuad();

	UnBind();
}

SSRBuffer::SSRBuffer()
{
	Initialize();
}

void SSRBuffer::Initialize()
{
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	// create floating point color buffer
	GenTexture();
	SetTextureParameters();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBOTexture, 0);

	// - Reflected UV Texture
	glGenTextures(1, &m_SSRReflectedUVs);
	glBindTexture(GL_TEXTURE_2D, m_SSRReflectedUVs);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SSRReflectedUVs, 0);

	// create depth buffer (renderbuffer)
	glGenRenderbuffers(1, &m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	// msaa 
	// attach buffers
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
	//glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA_SAMPLES, GL_DEPTH_COMPONENT, m_Width, m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		CHROMA_WARN("Framebuffer not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}
