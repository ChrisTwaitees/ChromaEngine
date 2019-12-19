#include "ShadowBuffer.h"
#include <component/ChromaMeshComponent.h>

void ShadowBuffer::calcLightSpaceMatrix()
{
	// Fetch sunlight from scene
	Light* SceneSunLight = m_scene->GetSunLight();
	// calculate LightSpaceMatrix
	float near_plane = 0.1f, far_plane = 35.0f;
	glm::mat4 lightProjection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(SceneSunLight->GetDirection() * -10.0f,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
}

void ShadowBuffer::Initialize()
{
	// create frame buffer to store depth to
	glGenFramebuffers(1, &depthMapFBO);
	// create texture to write shadow map to
	glGenTextures(1, &ShadowMapTexture.ID);
	glBindTexture(GL_TEXTURE_2D, ShadowMapTexture.ID);
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

	// shadow map texture type
	ShadowMapTexture.type = Texture::SHADOWMAP;
	for (IChromaEntity* entity : m_scene->GetEntities())
		for (IChromaComponent* component : entity->getMeshComponents())
		{
			((ChromaMeshComponent*)component)->AddTexture(ShadowMapTexture);
		}
}

void ShadowBuffer::bindShadowMapToBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMapTexture.ID, 0);
	// RBO are not complete without a color buffer
	// setting the following to NONE mitigates this
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowBuffer::calculateShadows()
{


	// Calculate LightSpace matrix
	calcLightSpaceMatrix();

	// Set uniforms on shader
	depthShader.use();
	depthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

	// Set map resolution
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Cull the front faces to prevent a dettached self 
	// shadowing effect, ** this is designed for opaque objects!
	// Attempt to alleviate Peter Panning
	//glCullFace(GL_FRONT);

	// render scene
	for (IChromaEntity* entity : m_scene->GetEntities())
	{
		glm::mat4 finalTransformMatrix = entity->GetTransformationMatrix();	
		for (IChromaComponent* component : entity->getShadowCastingComponents())
		{
			finalTransformMatrix *= ((ChromaMeshComponent*)component)->GetTransformationMatrix();
			depthShader.SetMat4("model", finalTransformMatrix);
			((ChromaMeshComponent*)component)->Draw(depthShader);

			// check if mesh skinned
			depthShader.setUniform("isSkinned", ((ChromaMeshComponent*)component)->m_IsSkinned);
			if (((ChromaMeshComponent*)component)->m_IsSkinned)
				((ChromaMeshComponent*)component)->SetJointUniforms(depthShader);

			// Draw Update Materials
			((ChromaMeshComponent*)component)->DrawUpdateMaterials(depthShader);
		}
	}

	unBind();

	//glCullFace(GL_BACK); // reset to original culling mode

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


ShadowBuffer::ShadowBuffer(ChromaScene*& Scene)
{
	m_scene = Scene;
	Initialize();
}


ShadowBuffer::~ShadowBuffer()
{
	Initialize();
}
