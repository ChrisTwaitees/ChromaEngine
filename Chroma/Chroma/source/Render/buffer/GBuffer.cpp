#include "GBuffer.h"
#include <component/MeshComponent.h>
#include <render/Render.h>
#include <shadow/ShadowBuffer.h>

namespace Chroma
{

	void GBuffer::Init()
	{
		// create the buffer
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		// SCENE DATA
		// - WS Positions
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Chroma::Render::GetWSPositions(), 0);

		// - VS Positions
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, Chroma::Render::GetVSPositions(), 0);

		// - fragposLightSpace 
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, Chroma::Render::GetFragPosLightSpace(), 0);

		// SURFACE DATA
		// - albebo 
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, Chroma::Render::GetAlbedo(), 0);

		// - WS Normals
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, Chroma::Render::GetWSNormals(), 0);

		// - VS Normals
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, Chroma::Render::GetVSNormals(), 0);

		// - metalness/rougness/ambient occlusion buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, Chroma::Render::GetMetRoughAO(), 0);
		
		// - depth
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Chroma::Render::GetDepth(), 0);

		// - tell OpenGL which color attachments we'll use for rendering 
		unsigned int attachments[7] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6};
		glDrawBuffers(7, attachments);
		// create and attach depth buffer (renderbuffer)
		glGenRenderbuffers(1, &m_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);

		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			CHROMA_WARN("GBUFFER:: Framebuffer not complete!");

		// second framebuffer with different attachments just for lighting
		glGenFramebuffers(1, &m_LightingFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_LightingFrameBuffer);

		// - direct lighting & shadows
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Chroma::Render::GetDirectLightingShadows(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, Chroma::Render::GetIndirectLighting(), 0);

		// create attachment buffers
		unsigned int lightingattachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glDrawBuffers(2, lightingattachments);

		// assign default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// configure shaders
		ConfigureShaders();
	}

	void GBuffer::SetTransformUniforms()
	{
		m_lightingPassShader.Use();
		m_lightingPassShader.SetUniform("scale", m_Scale);
		m_lightingPassShader.SetUniform("offset", m_Offset);
	}

	void GBuffer::ConfigureShaders()
	{
		// Geometry Buffer
		m_lightingPassShader.Use();
		m_lightingPassShader.SetUniform("gPosition", 0);
		m_lightingPassShader.SetUniform("gNormal", 1);
		m_lightingPassShader.SetUniform("gAlbedo", 2);
		m_lightingPassShader.SetUniform("gMetRoughAO", 3);
		m_lightingPassShader.SetUniform("gFragPosLightSpace", 4);
		m_lightingPassShader.SetUniform("gShadowmap", 5);
		m_lightingPassShader.SetUniform("SSAO", 6);
		m_lightingPassShader.SetUniform("gDepth", 7);
						 
		// IBL				 
		m_lightingPassShader.SetUniform("irradianceMap", 8);
		m_lightingPassShader.SetUniform("prefilterMap", 9);
		m_lightingPassShader.SetUniform("brdfLUT", 10);
	}

	void GBuffer::BindGBufferTextures()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Chroma::Render::GetWSPositions());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Chroma::Render::GetWSNormals());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, Chroma::Render::GetAlbedo());
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, Chroma::Render::GetMetRoughAO());
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, Chroma::Render::GetFragPosLightSpace());
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D_ARRAY, Chroma::Render::GetShadowBuffer()->GetTexture());
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, m_SSAOBuffer->GetTexture());
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, Chroma::Render::GetDepth());
		// IBL
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Chroma::Scene::GetIBL()->GetIrradianceMapID());
		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Chroma::Scene::GetIBL()->GetPrefilterMapID());
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, Chroma::Scene::GetIBL()->GetBRDFLUTID());
	}



	void GBuffer::DrawGeometryPass()
	{
		// 1. geometry pass: render scene's geometry/color data into gbuffer
		BindAndClear();
		m_geometryPassShader.Use();
		m_geometryPassShader.SetUniform("lightSpaceMatrix", static_cast<ShadowBuffer*>(Chroma::Render::GetShadowBuffer())->GetLightSpaceMatrix());

		// Render Lit Components
		for (UID const& uid : Chroma::Scene::GetLitComponentUIDs())
		{
			// transform components by entity transform
			m_geometryPassShader.SetUniform("model", static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->GetWorldTransform());

			// check if mesh skinned
			bool isSkinned = static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->GetIsSkinned();
			m_geometryPassShader.SetUniform("isSkinned", isSkinned);
			if (isSkinned)
				static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->SetJointUniforms(m_geometryPassShader);

			// Draw Update Materials
			// Check if Mesh is double sided
			if (static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->GetIsDoubleSided())
			{
				glDisable(GL_CULL_FACE);
				static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->DrawUpdateMaterials(m_geometryPassShader);
				glEnable(GL_CULL_FACE);
			}
			else // if not render one front facing
				static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->DrawUpdateMaterials(m_geometryPassShader);
		}
		UnBind();
	}


	void GBuffer::DrawLightingPass()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_LightingFrameBuffer);
		glClear(GL_COLOR_BUFFER_BIT);

		// use the lighting pass shader
		m_lightingPassShader.Use();
		// updating transforms
		SetTransformUniforms();
		// activating textures
		BindGBufferTextures();

		RenderQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GBuffer::BlitDepthBuffer()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_PostFXBuffer->GetFBO());// write to default HDR IFramebuffer
		glBlitFramebuffer(
			0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST
		);
	}

	void GBuffer::Draw()
	{
		// 1. geometry pass: render scene's geometry/color data into gbuffer
		DrawGeometryPass();

		// 1.5 SSAO Pass : draw SSAO in ViewSpace to be used during lighting pass
		static_cast<SSAOBuffer*>(m_SSAOBuffer)->Draw();

		// 2. Render pass to PostFX buffer
		//m_PostFXBuffer->Bind();

		// 3. lighting pass: calculate lighting using gbuffer textures

		DrawLightingPass();


		// 4. copy content of geometry's depth buffer to HDR buffer
		//CopyColorAndDepth(m_FBO, m_PostFXBuffer->GetFBO());
		CopyDepth(m_FBO, m_PostFXBuffer->GetFBO());

		// 5. Unbind postFX buffer
		//m_PostFXBuffer->UnBind();
	}

	void GBuffer::ResizeBuffers()
	{
		// resize SSAO
		m_SSAOBuffer->ScreenResizeCallback(m_Width, m_Height);

		// rbo
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);

	}


	GBuffer::GBuffer(IFramebuffer*& PostFXBuffer)
	{
		Init();
		m_PostFXBuffer = PostFXBuffer;
	}

	GBuffer::~GBuffer()
	{
	}
}
