#include "PostFXBuffer.h"
#include <render/Render.h>
#include <buffer/SSRBuffer.h>
#include <buffer/VXGIBuffer.h>
#include <buffer/GBuffer.h>

namespace Chroma
{
	void PostFXBuffer::Init()
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
		m_ScreenShader->SetUniform("u_Background", 0);
		m_ScreenShader->SetUniform("u_BloomTx", 1);
		m_ScreenShader->SetUniform("u_DirectLightingShadows", 2);
		m_ScreenShader->SetUniform("u_IndirectLighting", 3);
		m_ScreenShader->SetUniform("u_SSR", 4);
		m_ScreenShader->SetUniform("u_VXGI", 5);
		m_ScreenShader->SetUniform("u_VXReflectance", 6);
		m_ScreenShader->SetUniform("u_SSAO", 7);
		m_ScreenShader->SetUniform("u_MetRoughAO", 8);
		m_ScreenShader->SetUniform("u_ScreenPos", 9);
		
		SetTransformUniforms();
	}

	void PostFXBuffer::ResizeBuffers()
	{
		// textures
		glBindTexture(GL_TEXTURE_2D, colorBuffersTextures[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, colorBuffersTextures[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);

		// blur FBOs
		glBindTexture(GL_TEXTURE_2D, blurColorBuffers[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, blurColorBuffers[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL);

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw
		m_ScreenShader->Use();
		// textures

		// BG
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Render::GetForwardBuffer()->GetTexture());
		// Bloom
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, colorBuffersTextures[1]);
		// Direct lighting
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, Render::GetDirectLightingShadows());
		// Direct lighting
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, Render::GetIndirectLighting());
		// ssr
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, static_cast<SSRBuffer*>(Chroma::Render::GetSSRBuffer())->GetSSRReflectedUVTexture());
		// vxgi - irradiance
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, static_cast<VXGIBuffer*>(Chroma::Render::GetVXGIBuffer())->GetTexture());
		// vxgi - reflections
		//glActiveTexture(GL_TEXTURE6);
		//glBindTexture(GL_TEXTURE_2D, static_cast<SSRBuffer*>(Chroma::Render::GetSSRBuffer())->GetSSRReflectedUVTexture());
		// ssao
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, static_cast<GBuffer*>(Render::GetGBuffer())->GetSSAOTexture());
		// metalness roughness ao
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, Render::GetMetRoughAO());
		// depth
		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, Render::GetVSPositions());

		// uniforms
		m_ScreenShader->SetUniform("u_BloomAmount", m_BloomAmount);
		m_ScreenShader->SetUniform("u_IndirectContribution", m_IndirectContribution);
		m_ScreenShader->SetUniform("u_DirectContribution", m_DirectContribution);
		m_ScreenShader->SetUniform("u_SSRContribution", m_SSRContribution);
		m_ScreenShader->SetUniform("u_VXIrradiance_Contribution", m_VXIrradiance_Contribution);
		m_ScreenShader->SetUniform("u_VXRradiance_Contribution", m_VXRradiance_Contribution);
		m_ScreenShader->SetUniform("u_Exposure", m_Exposure);
		m_ScreenShader->SetUniform("u_Gamma", m_Gamma);
		m_ScreenShader->SetUniform("u_SSAOContribution", m_SSAOContribution);
		
		
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
			m_ScreenShader->Use();
			m_ScreenShader->SetUniform("u_Bloom", useBloom);
			Draw();
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
		Init();
	}

	PostFXBuffer::~PostFXBuffer()
	{
		delete blurShader;
		delete m_ScreenShader;
	}
}
