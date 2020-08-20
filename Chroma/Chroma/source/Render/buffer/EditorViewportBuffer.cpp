#include "EditorViewportBuffer.h"

namespace Chroma
{
	EditorViewportBuffer::EditorViewportBuffer()
	{
		Init();
	}

	EditorViewportBuffer::~EditorViewportBuffer()
	{
		delete m_ScreenShader;
	}

	void EditorViewportBuffer::Init()
	{
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		// create floating point color buffer
		GenTexture();
		SetTextureParameters();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBOTexture, 0);
		// Rebind default Framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void EditorViewportBuffer::ResizeBuffers()
	{
		// textures
		glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	}
}
