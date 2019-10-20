#include "Framebuffer.h"



void Framebuffer::setupQuad()
{
	int stride = 4;
	GLsizei verts_size = quadData.size() * sizeof(quadData[0]);
	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verts_size, &quadData[0], GL_STATIC_DRAW);

	// Vertex Shader Attribs
	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*)0);
	// uvs
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*)(sizeof(float) * 2));
}

void Framebuffer::GenTexture()
{
	glGenTextures(1, &FBOTexture);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_HEIGHT / resolutionFactor, SCREEN_WIDTH / resolutionFactor, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void Framebuffer::SetTextureParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Framebuffer::renderQuad()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);
}


void Framebuffer::initialize()
{
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

void Framebuffer::setTexture(unsigned int newFBOTexture)
{
	FBOTexture = newFBOTexture;
}

void Framebuffer::setResolutionScale(unsigned int newScale)
{
	resolutionFactor = newScale;
	int newWidth = SCREEN_WIDTH / resolutionFactor;
	int newHeight = SCREEN_HEIGHT / resolutionFactor;
}

void Framebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	screenShader->setVec2("scale", scale);
	screenShader->setVec2("offset", offset);
	renderQuad();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

Framebuffer::Framebuffer()
{
	initialize();
	setupQuad();
}


Framebuffer::~Framebuffer()
{
	delete screenShader;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
