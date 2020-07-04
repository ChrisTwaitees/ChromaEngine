#include "PostFXBuffer.h"
#include <screen/Screen.h>
#include <render/Render.h>
#include <buffer/SSRBuffer.h>

namespace Chroma
{
	void PostFXBuffer::Initialize()
	{
		m_ScreenShader = new Shader(fragSource, vtxSource);
		blurShader = new Shader(blurfragSource, vtxSource);
		// generate and Bind frame buffers
		glGenFramebuffers(1, &hdrFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		// generate color buffers
		glGenTextures(2, colorBuffersTextures);
		for (unsigned int i = 0 ; i < 2; i++)
		{
			// Bind textures
			glBindTexture(GL_TEXTURE_2D, colorBuffersTextures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
			// set texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			// attach texture to framebuffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffersTextures[i], 0);
		}
		// create depth buffer (renderbuffer)
		glGenRenderbuffers(1, &m_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
		// pass color attachments in order for multiple color buffers to be written
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);
		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			CHROMA_WARN("POSTFX BUFFER:: Framebuffer not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// generate Blur Buffers and textures
		genBlurBuffer();

		// configure shaders
		ConfigureShaders();
	}

	void PostFXBuffer::SetTransformUniforms()
	{
		blurShader->Use();
		blurShader->SetUniform("scale", m_Scale);
		blurShader->SetUniform("offset", m_Offset);
		m_ScreenShader->Use();
		m_ScreenShader->SetUniform("scale", m_Scale);
		m_ScreenShader->SetUniform("offset", m_Offset);
	}

	void PostFXBuffer::ConfigureShaders()
	{
		blurShader->Use();
		blurShader->SetUniform("image", 0);
		m_ScreenShader->Use();
		m_ScreenShader->SetUniform("scene", 0);
		m_ScreenShader->SetUniform("bloomBlur", 1);
		m_ScreenShader->SetUniform("ssr", 2);

		SetTransformUniforms();
	}

	void PostFXBuffer::ResizeBuffers()
	{
		// textures
		glBindTexture(GL_TEXTURE_2D, colorBuffersTextures[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, colorBuffersTextures[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);

		// rbo
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
		// attach buffers
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
	}


	void PostFXBuffer::genBlurBuffer()
	{
		// ping-pong-framebuffer for blurring
		glGenFramebuffers(2, blurFBOs);
		glGenTextures(2, blurColorBuffers);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, blurFBOs[i]);
			glBindTexture(GL_TEXTURE_2D, blurColorBuffers[i]);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurColorBuffers[i], 0
			);
			// also check if framebuffers are complete (no need for depth buffer)
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				CHROMA_WARN("POSTFX BUFFER:: Blur Framebuffer not complete!");
		}
	}

	void PostFXBuffer::blurFragments()
	{
		UnBind();
		horizontal = true, first_iteration = true;
		blurShader->Use();
		for (int i = 0; i < blurIterations; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, blurFBOs[horizontal]);
			blurShader->SetUniform("horizontal", horizontal);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(
				GL_TEXTURE_2D, first_iteration ? colorBuffersTextures[1] : blurColorBuffers[!horizontal]
			);
			RenderQuad();
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;

		}
		UnBind();
	}

	void PostFXBuffer::Draw()
	{
		// Set to default buffer
		UnBind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw
		m_ScreenShader->Use();
		// textures
		// scene texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffersTextures[0]);
		// ssr
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, static_cast<SSRBuffer*>(Chroma::Render::GetSSRBuffer())->GetSSRReflectedUVTexture());
		// bloom
		m_ScreenShader->SetUniform("bloom", true);
		// setting transform uniforms
		SetTransformUniforms();
		RenderQuad();
	}

	void PostFXBuffer::Draw(const bool& useBloom)
	{
		if (useBloom)
		{
			// blur BrightFragments
			UnBind();
			bool horizontal = true, first_iteration = true;
			blurShader->Use();
			for (int i = 0; i < blurIterations; i++)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, blurFBOs[horizontal]);
				blurShader->SetUniform("horizontal", horizontal);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(
					GL_TEXTURE_2D, first_iteration ? colorBuffersTextures[1] : blurColorBuffers[!horizontal]
				);
				RenderQuad();
				horizontal = !horizontal;
				if (first_iteration)
					first_iteration = false;
			}
			UnBind();
			// Composite blur and HDR and tone
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_ScreenShader->Use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, colorBuffersTextures[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, blurColorBuffers[!horizontal]);
			m_ScreenShader->SetUniform("bloom", useBloom);
			// setting transform uniforms
			RenderQuad();
		}
		else
			Draw();

	}

	void PostFXBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	PostFXBuffer::PostFXBuffer()
	{
		Initialize();
	}

	PostFXBuffer::~PostFXBuffer()
	{
		delete blurShader;
		delete m_ScreenShader;
	}
}
