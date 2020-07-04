#include "IFramebuffer.h"


namespace Chroma
{
	void IFramebuffer::ResizeBuffers()
	{
		// textures
		glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

		// rbo
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
		// attach buffers
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
	}

	void IFramebuffer::GenTexture()
	{
		glGenTextures(1, &m_FBOTexture);
		glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	}

	void IFramebuffer::SetTextureParameters()
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void IFramebuffer::SetTransformUniforms()
	{
		m_ScreenShader->SetUniform("scale", m_Scale);
		m_ScreenShader->SetUniform("offset", m_Offset);
	}

	void IFramebuffer::CopyColorAndDepth(unsigned int const& sourceFBO, unsigned int const& targetFBO)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, sourceFBO); // fetch
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFBO); // copy 
		glBlitFramebuffer(
			0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST
		);
		glBlitFramebuffer(
			0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST
		);
	}

	void IFramebuffer::CopyDepth(unsigned int const& sourceFBO, unsigned int const& targetFBO)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, sourceFBO); // fetch 
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFBO); // copy 
		glBlitFramebuffer(
			0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST
		);
	}

	void IFramebuffer::CopyColor(unsigned int const& sourceFBO, unsigned int const& targetFBO)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, sourceFBO); // fetch  
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFBO); // copy 
		glBlitFramebuffer(
			0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST
		);
	}

	void IFramebuffer::RenderQuad()
	{
		m_Quad.BindDrawVAO();
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
		// msaa 
		// attach buffers
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
		//glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA_SAMPLES, GL_DEPTH_COMPONENT, m_Width, m_Height);
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

	void IFramebuffer::ClearColorAndDepth()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void IFramebuffer::Draw()
	{
		m_ScreenShader->Use();
		m_ScreenShader->SetUniform("screenTexture", 0);
		// draw
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// using color attachment
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_FBOTexture);
		// setting transform uniforms
		SetTransformUniforms();
		RenderQuad();
	}

	void IFramebuffer::ScreenResizeCallback(int const& width, int const& height)
	{
		m_Width = width;
		m_Height = height;
		ResizeBuffers();
	
	}


	IFramebuffer::IFramebuffer()
	{
		Initialize();
	}


	IFramebuffer::~IFramebuffer()
	{
		delete m_ScreenShader;
	}
}


