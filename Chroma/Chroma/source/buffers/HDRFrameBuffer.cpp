#include "HDRFrameBuffer.h"



void HDRFramebuffer::GenTexture()
{
	glGenTextures(1, &FBOTexture);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_HEIGHT, SCREEN_WIDTH, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

void HDRFramebuffer::initialize()
{
	screenShader = new Shader(fragSource, vtxSource);

	glGenFramebuffers(1, &FBO);
	// create floating point color buffer
	GenTexture();
	SetTextureParameters();
	// create depth buffer (renderbuffer)
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	// attach buffers
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HDRFramebuffer::Draw()
{
	// using color attachment
	screenShader->use();
	// draw
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	// setting transform uniforms
	screenShader->setVec2("scale", scale);
	screenShader->setVec2("offset", offset);
	//screenShader.setFloat("exposure", exposure);
	renderQuad();
}

void HDRFramebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

HDRFramebuffer::HDRFramebuffer()
{
	initialize();
}


HDRFramebuffer::~HDRFramebuffer()
{
}
