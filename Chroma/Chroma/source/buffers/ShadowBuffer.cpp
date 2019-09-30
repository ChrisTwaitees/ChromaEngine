#include "ShadowBuffer.h"

void ShadowBuffer::initialize()
{
	// depth shader
	depthShader = Shader(depthFragSource, depthVtxSource);

	// create frame buffer to store depth to
	glGenFramebuffers(1, &depthMapFBO);
	// create texture to write shadow map to
	glGenTextures(1, &texShadowMap);
	glBindTexture(GL_TEXTURE_2D, texShadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
		width, height, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


void ShadowBuffer::calculateShadows(const Light& light)
{
	// calculate LightSpaceMatrix
	if (light.type == light.DIRECTIONAL)
	{
		float near_plane = 1.0f, far_plane = 7.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(light.direction * 10.0f,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;
	}

	// set uniforms and bind to buffer
	depthShader.use();
	depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	// render the scene with depthshader
	// bind the original glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
}

void ShadowBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texShadowMap, 0);
	// RBO are not complete without a color buffer
	// setting the following to NONE mitigates this
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowBuffer::ShadowBuffer()
{
	initialize();
}


ShadowBuffer::~ShadowBuffer()
{
}
