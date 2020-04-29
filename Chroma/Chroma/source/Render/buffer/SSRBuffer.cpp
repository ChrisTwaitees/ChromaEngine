#include "SSRBuffer.h"
#include <render/Render.h>
#include <buffer/GBuffer.h>

void SSRBuffer::Draw()
{
	Bind();
	// Bind Textures
	// 0 VsNormals
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Chroma::Render::GetVSNormals());
	// 1 VsPositions
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Chroma::Render::GetVSPositions());
	// 2 testColor
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, Chroma::Render::GetAlbedo());

	// Draw
	m_ScreenShader->Use();
	// uniforms
	m_ScreenShader->SetUniform("vProjection", Chroma::Scene::GetRenderCamera()->GetProjectionMatrix());
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

	// - Reflected UV Texture
	glGenTextures(1, &m_SSRReflectedUVs);
	glBindTexture(GL_TEXTURE_2D, m_SSRReflectedUVs);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SSRReflectedUVs, 0);

	//// create depth buffer (renderbuffer)
	//glGenRenderbuffers(1, &m_RBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);

	//// attach buffers
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		CHROMA_WARN("Framebuffer not complete!");

	// reset to default framebuffer
	UnBind();

	// Set Shader Uniforms
	m_ScreenShader->Use();
	m_ScreenShader->SetUniform("vsNormals", 0);
	m_ScreenShader->SetUniform("vsPositions", 1);
	m_ScreenShader->SetUniform("vAlbedo", 2);

}

void SSRBuffer::ResizeBuffers()
{
	glBindTexture(GL_TEXTURE_2D, m_SSRReflectedUVs);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);

}
