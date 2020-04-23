#include "ShadowBuffer.h"
#include <component/MeshComponent.h>
#include <screen/Screen.h>

void ShadowBuffer::calcLightSpaceMatrix()
{
	// calculate LightSpaceMatrix
	float near_plane = 0.01f, far_plane = 50.0f;
	glm::mat4 lightProjection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(Chroma::Scene::GetRenderCamera()->GetPosition() + (Chroma::Scene::GetSunLight()->GetDirection() * -15.0f),
		Chroma::Scene::GetRenderCamera()->GetPosition(),
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 	width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// bindShadowMapToBuffer the texture to the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMapTexture.ID, 0);
	// m_RBO are not complete without a color buffer
	// setting the following to NONE mitigates this
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// shadow map texture type
	ShadowMapTexture.m_Type = Texture::SHADOWMAP;

	// Attach shadowmap to all shadowcasting elements in scene
	BindShadowMaps();
}

void ShadowBuffer::ResizeBuffers()
{
	// Resize to half the screen resolution
	width = Chroma::Screen::GetWidthHeight().first;
	height = Chroma::Screen::GetWidthHeight().second;

	// textures
	glBindTexture(GL_TEXTURE_2D, ShadowMapTexture.ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

}


void ShadowBuffer::DrawShadowMaps()
{
	// Calculate LightSpace matrix
	calcLightSpaceMatrix();

	// Set uniforms on shader
	depthShader.Use();
	depthShader.SetUniform("lightSpaceMatrix", lightSpaceMatrix);

	// Set map resolution
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Cull the front faces to prevent a dettached self 
	// shadowing effect, ** this is designed for opaque objects!
	// Attempt to alleviate Peter Panning
	//glCullFace(GL_FRONT);

	// render scene
	for (UID const& uid : Chroma::Scene::GetShadowCastingComponentUIDs())
	{
		depthShader.SetUniform("model", static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->GetWorldTransform());
		
		// check if mesh skinned
		bool isSkinned = static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->GetIsSkinned();
		depthShader.SetUniform("isSkinned", isSkinned);
		if (isSkinned)
			static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->SetJointUniforms(depthShader);

		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->Draw(depthShader);
	}

	UnBind();

	//glCullFace(GL_BACK); // reset to original culling mode

	glViewport(0, 0, Chroma::Screen::GetWidthHeight().first, Chroma::Screen::GetWidthHeight().second);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ShadowBuffer::BindShadowMaps()
{
	for (UID const& uid : Chroma::Scene::GetShadowCastingComponentUIDs())
	{
		static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->AddTexture(ShadowMapTexture);
	}
}


ShadowBuffer::ShadowBuffer()
{
	Initialize();
}


ShadowBuffer::~ShadowBuffer()
{
}
