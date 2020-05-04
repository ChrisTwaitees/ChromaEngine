#include "ShadowBuffer.h"
#include <component/MeshComponent.h>
#include <screen/Screen.h>

void ShadowBuffer::BuildCSMTextureArray()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_CascadedTexureArray);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, m_ShadowMapSize, m_ShadowMapSize, m_NumSplits, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void ShadowBuffer::CalcCascadeSplitDistances()
{
	//for (unsigned int i = 0; i < m_NumSplits; i++)
	//{
	//	float p = (i + 1) / static_cast<float>(m_NumSplits);
	//	float log = minZ * std::pow(ratio, p);
	//	float uniform = minZ + range * p;
	//	float d = lambda * (log - uniform) + uniform;
	//	m_CascadeSplitDistances[i] = (d - nearClip) / clipRange;
	//}
}

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
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// create cascade texture array
	glGenTextures(1, &m_CascadedTexureArray);
	BuildCSMTextureArray();

	// attach depth texture to depth buffer attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_CascadedTexureArray, 0);
	
	// m_RBO are not complete without a color buffer
	// setting the following to NONE mitigates this
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Check status of frambuffer
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) 
		CHROMA_ERROR("SHADOWBUFFER :: Buffer Failed to complete!");

	// restore default FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// build split distances
	CalcCascadeSplitDistances();

	//
}

void ShadowBuffer::ResizeBuffers()
{
	//// Resize to half the screen resolution
	//width = Chroma::Screen::GetWidthHeight().first;
	//height = Chroma::Screen::GetWidthHeight().second;

	//// textures
	//glBindTexture(GL_TEXTURE_2D, m_ShadowMapTexture.ID);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width *2.0, height * 2.0, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

}


void ShadowBuffer::DrawShadowMaps()
{
	// Calculate LightSpace matrix
	calcLightSpaceMatrix();

	// Set uniforms on shader
	depthShader.Use();
	depthShader.SetUniform("lightSpaceMatrix", lightSpaceMatrix);

	// Set map resolution
	//glViewport(0, 0, width * 2.0, height * 2.0);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
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


ShadowBuffer::ShadowBuffer()
{
	Initialize();
}


ShadowBuffer::~ShadowBuffer()
{
}
