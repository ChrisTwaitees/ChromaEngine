#include "Framebuffer.h"



void Framebuffer::initialize()
{
	// generate and bind to operate on
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// generate texture to write to
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, )
}

Framebuffer::Framebuffer()
{
}


Framebuffer::~Framebuffer()
{
}
