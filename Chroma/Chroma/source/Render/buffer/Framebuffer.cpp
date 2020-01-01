#include "Framebuffer.h"



void Framebuffer::GenTexture()
{
	glGenTextures(1, &FBOTexture);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_HEIGHT, SCREEN_WIDTH, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void Framebuffer::SetTextureParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Framebuffer::updateTransformUniforms()
{
	screenShader->setVec2("scale", scale);
	screenShader->setVec2("offset", offset);
}

void Framebuffer::renderQuad()
{
	quad.BindDrawVAO();
}



void Framebuffer::Initialize()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// create floating point color buffer
	GenTexture();
	SetTextureParameters();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOTexture, 0);
	// create depth buffer (renderbuffer)
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	// attach buffers
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		CHROMA_WARN("Framebuffer not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


void Framebuffer::setTexture(unsigned int newFBOTexture)
{
	FBOTexture = newFBOTexture;
}


void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}


void Framebuffer::unBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::ClearBuffer()
{
	Bind();
	unBind();
}

void Framebuffer::Draw()
{
	screenShader->use();
	// draw
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// using color attachment
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	// setting transform uniforms
	updateTransformUniforms();
	renderQuad();
}


Framebuffer::Framebuffer()
{
	Initialize();
}


Framebuffer::~Framebuffer()
{
	delete screenShader;
}
