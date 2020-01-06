#include "IFramebuffer.h"



void IFramebuffer::GenTexture()
{
	glGenTextures(1, &m_FBOTexture);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_HEIGHT, SCREEN_WIDTH, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void IFramebuffer::SetTextureParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void IFramebuffer::UpdateTransformUniforms()
{
	m_ScreenShader->setVec2("scale", m_Scale);
	m_ScreenShader->setVec2("offset", m_Offset);
}

void IFramebuffer::CopyColorAndDepth(unsigned int const& sourceFBO, unsigned int const& targetFBO)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, sourceFBO); // fetch
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFBO); // copy 
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST
	);
}

void IFramebuffer::CopyDepth(unsigned int const& sourceFBO, unsigned int const& targetFBO)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, sourceFBO); // fetch 
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFBO); // copy 
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
}

void IFramebuffer::CopyColor(unsigned int const& sourceFBO, unsigned int const& targetFBO)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, sourceFBO); // fetch  
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFBO); // copy 
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST
	);
}

void IFramebuffer::RenderQuad()
{
	quad.BindDrawVAO();
}



void IFramebuffer::Initialize()
{
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	// create floating point color buffer
	GenTexture();
	SetTextureParameters();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBOTexture, 0);
	// create depth buffer (renderbuffer)
	glGenRenderbuffers(1, &m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	// attach buffers
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		CHROMA_WARN("Framebuffer not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


void IFramebuffer::SetTexture(unsigned int newFBOTexture)
{
	m_FBOTexture = newFBOTexture;
}


void IFramebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void IFramebuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void IFramebuffer::ClearBuffer()
{
	Bind();
	UnBind();
}

void IFramebuffer::Draw()
{
	m_ScreenShader->use();
	// draw
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// using color attachment
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
	// setting transform uniforms
	UpdateTransformUniforms();
	RenderQuad();
}


IFramebuffer::IFramebuffer()
{
	Initialize();
}


IFramebuffer::~IFramebuffer()
{
	delete m_ScreenShader;
}


