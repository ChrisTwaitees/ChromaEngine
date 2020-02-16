#include "PostFXBuffer.h"


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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_HEIGHT, SCREEN_WIDTH, 0, GL_RGBA, GL_FLOAT, NULL);
		// set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffersTextures[i], 0);
	}
	// create depth buffer (renderbuffer)
	glGenRenderbuffers(1, &hdrRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, hdrRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrRBO);
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
	configure_shaders();
}

void PostFXBuffer::UpdateTransformUniforms()
{
	blurShader->Use();
	blurShader->SetVec2("scale", m_Scale);
	blurShader->SetVec2("offset", m_Offset);
	m_ScreenShader->Use();
	m_ScreenShader->SetVec2("scale", m_Scale);
	m_ScreenShader->SetVec2("offset", m_Offset);
}

void PostFXBuffer::configure_shaders()
{
	blurShader->Use();
	blurShader->SetInt("image", 0);
	m_ScreenShader->Use();
	m_ScreenShader->SetInt("scene", 0);
	m_ScreenShader->SetInt("bloomBlur", 1);
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
			GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL
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
		blurShader->SetInt("horizontal", horizontal);
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
	UnBind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_ScreenShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffersTextures[0]);
	m_ScreenShader->SetInt("bloom", 0);
	// setting transform uniforms
	UpdateTransformUniforms();
	RenderQuad();
}

void PostFXBuffer::Draw(const bool& useBloom)
{
	UpdateTransformUniforms();

	if (useBloom)
	{
		// blur BrightFragments
		UnBind();
		bool horizontal = true, first_iteration = true;
		blurShader->Use();
		for (int i = 0; i < blurIterations; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, blurFBOs[horizontal]);
			blurShader->SetInt("horizontal", horizontal);
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
		m_ScreenShader->SetInt("bloom", useBloom);
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
