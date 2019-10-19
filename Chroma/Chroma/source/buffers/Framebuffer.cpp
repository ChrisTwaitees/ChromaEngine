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


void Framebuffer::initialize()
{
	// generate framebuffer and bindShadowMapToBuffer to operate on
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// generate texture to write to
	glGenTextures(1, &FBOTexture);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_HEIGHT / resolutionFactor, SCREEN_WIDTH / resolutionFactor, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOTexture, 0);
	// attaching the stencil and depth buffers to RenderBufferObject (remember we don't need to sample)
	// (so a RenderBufferObject is perfect for this purpose)
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_HEIGHT / resolutionFactor, SCREEN_WIDTH / resolutionFactor);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// once we've allocated enought memory for the render buffer we can unbind
	// to complete the frame buffer, we attache to the render buffer object to the depth
	// and stencil attachment of the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	// checking whether the creation of the Framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// to then render to the newly created framebuffer we simply bindShadowMapToBuffer it
	// all subsequent rendering commands with then influence the currently 
	// bound framebuffer

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
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void Framebuffer::Draw()
{
	// draw
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);

	// using color attachment
	screenShader.use();
	// setting transform uniforms
	screenShader.setVec2("scale", scale);
	screenShader.setVec2("offset", offset);
	glBindVertexArray(VAO);
	//glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glEnable(GL_DEPTH_TEST);
}

Framebuffer::Framebuffer()
{
	initialize();
	setupQuad();
}


Framebuffer::~Framebuffer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
