#include "HDRFrameBuffer.h"


void HDRFramebuffer::initialize()
{
	screenShader = new Shader(fragSource, vtxSource);
	blurShader = new Shader(blurfragSource, vtxSource);
	// generate and bind frame buffers
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	// generate color buffers
	glGenTextures(2, colorBuffersTextures);
	for (unsigned int i = 0 ; i < 2; i++)
	{
		// bind textures
		glBindTexture(GL_TEXTURE_2D, colorBuffersTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_HEIGHT, SCREEN_WIDTH, 0, GL_RGB, GL_FLOAT, NULL);
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
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// generate Blur Buffers and textures
	genBlurBuffer();

	// configure shaders
	configure_shaders();
}

void HDRFramebuffer::configure_shaders()
{
	blurShader->use();
	blurShader->setInt("image", 0);
	screenShader->use();
	screenShader->setInt("scene", 0);
	screenShader->setInt("bloomBlur", 1);
}

void HDRFramebuffer::genBlurBuffer()
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
			std::cout << "Framebuffer not complete!" << std::endl;
	}
}

void HDRFramebuffer::blurFragments()
{
	unBind();
	horizontal = true, first_iteration = true;
	blurShader->use();
	for (unsigned int i = 0; i < blurIterations; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBOs[horizontal]);
		blurShader->setInt("horizontal", horizontal);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(
			GL_TEXTURE_2D, first_iteration ? colorBuffersTextures[1] : blurColorBuffers[!horizontal]
		);
		renderQuad();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;

	}
	unBind();
}


void HDRFramebuffer::Draw()
{
	unBind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screenShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffersTextures[0]);
	screenShader->setInt("bloom", 0);
	//screenShader->setFloat("exposure", exposure);
	// setting transform uniforms
	screenShader->setVec2("scale", scale);
	screenShader->setVec2("offset", offset);
	renderQuad();
}

void HDRFramebuffer::Draw(bool useBloom)
{
	if (useBloom)
	{
		// blur BrightFragments
		unBind();
		horizontal = true, first_iteration = true;
		blurShader->use();
		for (unsigned int i = 0; i < blurIterations; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, blurFBOs[horizontal]);
			blurShader->setInt("horizontal", horizontal);
			//glActiveTexture(GL_TEXTURE0);
			glBindTexture(
				GL_TEXTURE_2D, first_iteration ? colorBuffersTextures[1] : blurColorBuffers[!horizontal]
			);

			renderQuad();
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;

		}
		unBind();
		//unBind();
		// Composite blur and HDR and tone
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		screenShader->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffersTextures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, blurColorBuffers[!horizontal]);
		screenShader->setInt("bloom", useBloom);
		//screenShader->setFloat("exposure", exposure);
		// setting transform uniforms
		screenShader->setVec2("scale", scale);
		screenShader->setVec2("offset", offset);
		renderQuad();
	}
	else
	{
		Draw();
	}
		
}

void HDRFramebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


HDRFramebuffer::HDRFramebuffer()
{
	initialize();
}


HDRFramebuffer::~HDRFramebuffer()
{
	delete blurShader;
	delete screenShader;
}
