#include "ForwardBuffer.h"
#include <render/Render.h>
#include <component/UIComponent.h>

namespace Chroma
{
	void ForwardBuffer::Init()
	{
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		// create floating point color buffer
		glGenTextures(1, &m_FBOTexture);
		glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBOTexture, 0);
		// create depth buffer (renderbuffer)
		glGenRenderbuffers(1, &m_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
		// attach buffers
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			CHROMA_WARN("Framebuffer not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}	

	void ForwardBuffer::RenderForwardComponents()
	{
		// attach current buffer and copy contents of postfx buffer
		BindAndClear();
		CopyColorAndDepth(m_PostFXBuffer->GetFBO(), m_FBO);

		// Render Skybox first for Transparent Entities
		Chroma::Scene::GetSkyBox()->Draw();

		// Render Unlit Components
		for (UID const& uid : Chroma::Scene::GetUnlitComponentUIDs())
		{
			// render unlit components
			static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->DrawUpdateTransforms(*Chroma::Scene::GetRenderCamera());
		}

		// Render Forward Lit Components
		for (UID const& uid : Chroma::Scene::GetForwardLitComponentUIDs())
		{
			// Check whether component is transparent
			if (static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->GetIsTransparent())
				continue;
			// update the light view matrix
			static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->SetMat4("lightSpaceMatrix", Chroma::Render::GetLightSpaceMatrix());
			// render forward lit components
			static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->DrawUpdateTransforms(*Chroma::Scene::GetRenderCamera());

		}

		// Render Transparent Components
		if(Chroma::Scene::GetTransparentComponentUIDs().size() > 0)
			RenderTransparency();
	}

	void ForwardBuffer::RenderTransparency()
	{
		// Set to alpha blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Disable Back Face Culling to allow interior of transparent objects to be seen
		glDisable(GL_CULL_FACE);

		// Sorting for Transparency Shading
		std::map<float, UID> alpha_sorted;
		for (UID const& uid : Chroma::Scene::GetTransparentComponentUIDs())
		{
			float distance = Chroma::Scene::GetMeshComponentDistanceToCamera(uid);
			alpha_sorted[distance] = uid;
		}
		// iterating from furthest to closest
		for (std::map<float, UID>::reverse_iterator it = alpha_sorted.rbegin(); it != alpha_sorted.rend(); ++it)
		{
			for (UID const& uid : Chroma::Scene::GetTransparentComponentUIDs())
			{
				if (static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->GetIsForwardLit()) // draw lit transparent components
					static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->Draw(*Chroma::Scene::GetRenderCamera());
				else // draw unlit transparent components
					static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->DrawUpdateTransforms(*Chroma::Scene::GetRenderCamera());
			}
		}

		// set to default blending
		glBlendFunc(GL_ONE, GL_ZERO);
		// Re enable backface culling for preventing unecessary rendering
		glEnable(GL_CULL_FACE);
	}


	void ForwardBuffer::DrawQuad()
	{
		// use screen shader
		m_ScreenShader->Use();
		// using color attachment
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
		// setting transform uniforms
		SetTransformUniforms();
		RenderQuad();
	}

	void ForwardBuffer::Draw()
	{
		// 1. Render Forward Components to m_FBO
		RenderForwardComponents();

		// 2. Copy Color and Depth from Forward Buffer to 
		// Post FX Buffer
		CopyColorAndDepth(m_FBO, m_PostFXBuffer->GetFBO());

		// 3. Set back to default buffer
		UnBind();
	}

	void ForwardBuffer::ResizeBuffers()
	{
		// textures
		glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);

		// rbo
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
		// attach buffers
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
	}

	ForwardBuffer::ForwardBuffer(IFramebuffer* const& postFXBuffer)
	{
		m_PostFXBuffer = postFXBuffer;
	}

	ForwardBuffer::~ForwardBuffer()
	{
	}
}