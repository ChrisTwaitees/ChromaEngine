#include "ShadowBuffer.h"

void ShadowBuffer::calcLightSpaceMatrix()
{
	// Fetch sunlight from scene
	Light* SceneSunLight = mScene->SunLight;
	// calculate LightSpaceMatrix
	float near_plane = 1.0f, far_plane = 15.0f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(SceneSunLight->direction * -10.0f,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
}

void ShadowBuffer::initialize()
{
	// shadow map texture type
	ShadowMapTexture.type = Texture::SHADOWMAP;

	// depth shader
	depthShader = Shader(depthFragSource, depthVtxSource);

	// create frame buffer to store depth to
	glGenFramebuffers(1, &depthMapFBO);
	// create texture to write shadow map to
	glGenTextures(1, &ShadowMapTexture.id);
	glBindTexture(GL_TEXTURE_2D, ShadowMapTexture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
		width, height, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// bindShadowMapToBuffer the texture to the framebuffer
	bindShadowMapToBuffer();
}

void ShadowBuffer::bindShadowMapToBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMapTexture.id, 0);
	// RBO are not complete without a color buffer
	// setting the following to NONE mitigates this
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowBuffer::calculateShadows()
{
	// Cull the front faces to prevent a dettached self 
	// shadowing effect, ** this is designed for opaque objects!
	glCullFace(GL_FRONT);

	// Calculate LightSpace matrix
	calcLightSpaceMatrix();

	// Set uniforms on shader
	depthShader.use();
	depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	// Set map resolution
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	// render scene
	for (ChromaEntity* entity : mScene->Entities)
	{
		glm::mat4 finalTransformMatrix = entity->getTransformationMatrix();	
		for (ChromaComponent* component : entity->RenderableComponents)
		{
			finalTransformMatrix *= component->getTransformationMatrix();
			depthShader.setMat4("model", finalTransformMatrix);
			component->Draw(depthShader);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCullFace(GL_BACK); // reset to original culling mode

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



ShadowBuffer::ShadowBuffer(ChromaScene* Scene)
{
	mScene = Scene;
	initialize();
}


ShadowBuffer::~ShadowBuffer()
{
	initialize();
}