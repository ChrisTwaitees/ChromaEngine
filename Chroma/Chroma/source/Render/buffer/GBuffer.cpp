#include "GBuffer.h"
#include <component/MeshComponent.h>

void GBuffer::Initialize()
{
	// create the buffer
	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

	// SCENE DATA
	// - position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// - gViewPosition for SSAO viewspace position 
	glGenTextures(1, &gViewPosition);
	glBindTexture(GL_TEXTURE_2D, gViewPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gViewPosition, 0);

	// - fragposLightSpace color buffer for shadowmapping
	glGenTextures(1, &gFragPosLightSpace);
	glBindTexture(GL_TEXTURE_2D, gFragPosLightSpace);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gFragPosLightSpace, 0);

	// SURFACE DATA
	// - albebo buffer
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gAlbedo, 0);

	// - normal buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gNormal, 0);

	// - gViewPosition for SSAO viewspace position 
	glGenTextures(1, &gViewNormal);
	glBindTexture(GL_TEXTURE_2D, gViewNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, gViewNormal, 0);

	// - metalness/rougness/ambient occlusion buffer
	glGenTextures(1, &gMetRoughAO);
	glBindTexture(GL_TEXTURE_2D, gMetRoughAO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, gMetRoughAO, 0);

	// - tell OpenGL which color attachments we'll use for rendering 
	unsigned int attachments[7] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6 };
	glDrawBuffers(7, attachments);
	// create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &m_gRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_gRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gRBO);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		CHROMA_WARN("GBUFFER:: Framebuffer not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// configure shaders
	configureShaders();
}

void GBuffer::UpdateTransformUniforms()
{
	m_lightingPassShader.use();
	m_lightingPassShader.setVec2("scale", m_Scale);
	m_lightingPassShader.setVec2("offset", m_Offset);
}

void GBuffer::configureShaders()
{
	// Geometry Buffer
	m_lightingPassShader.use();
	m_lightingPassShader.SetInt("gPosition", 0);
	m_lightingPassShader.SetInt("gNormal", 1);
	m_lightingPassShader.SetInt("gAlbedo", 2);
	m_lightingPassShader.SetInt("gMetRoughAO", 3);
	m_lightingPassShader.SetInt("gFragPosLightSpace", 4);
	m_lightingPassShader.SetInt("gShadowmap", 5);
	m_lightingPassShader.SetInt("SSAO", 6);

	// IBL
	m_lightingPassShader.SetInt("irradianceMap", 7);
	m_lightingPassShader.SetInt("prefilterMap", 8);
	m_lightingPassShader.SetInt("brdfLUT", 9);
}

void GBuffer::bindAllGBufferTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gMetRoughAO);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gFragPosLightSpace);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, mShadowbuffer->GetTexture());
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_SSAOBuffer->GetTexture());
	// IBL
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Chroma::Scene::GetIBL()->getIrradianceMapID());
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Chroma::Scene::GetIBL()->getPrefilterMapID());
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, Chroma::Scene::GetIBL()->getBRDFLUTID());
}

void GBuffer::setLightingUniforms()
{
	m_lightingPassShader.setLightingUniforms(Chroma::Scene::GetLights(), *Chroma::Scene::GetRenderCamera());
	m_lightingPassShader.SetUniform("ambient", Chroma::Scene::GetAmbientColor());
}

void GBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GBuffer::calculateShadows()
{
	// 1. calculate shadows
	mShadowbuffer->calculateShadows();
}


void GBuffer::drawGeometryPass()
{
	// 1. geometry pass: render scene's geometry/color data into gbuffer
	Bind();
	m_geometryPassShader.use();
	m_geometryPassShader.SetMat4("view", Chroma::Scene::GetRenderCamera()->GetViewMatrix());
	m_geometryPassShader.SetMat4("projection", Chroma::Scene::GetRenderCamera()->GetProjectionMatrix());
	m_geometryPassShader.SetMat4("lightSpaceMatrix", mShadowbuffer->getLightSpaceMatrix());

	// Render Scene
	for (std::string const& UID : Chroma::Scene::GetEntityUIDs())
	{
		glm::mat4 finalTransformMatrix = Chroma::Scene::GetEntity(UID)->GetTransform();
		for (IComponent* component : Chroma::Scene::GetEntity(UID)->getLitComponents())
		{
			// transform components by entity transform
			finalTransformMatrix = finalTransformMatrix * ((MeshComponent*)component)->GetTransform();
			m_geometryPassShader.SetMat4("model", finalTransformMatrix);

			// check if mesh skinned
			m_geometryPassShader.SetUniform("isSkinned", ((MeshComponent*)component)->m_IsSkinned);
			if (((MeshComponent*)component)->m_IsSkinned)
				((MeshComponent*)component)->SetJointUniforms(m_geometryPassShader);

			// Draw Update Materials
			((MeshComponent*)component)->DrawUpdateMaterials(m_geometryPassShader);
		}
	}
	UnBind();
}

void GBuffer::RenderWithShader(IEntity* Entity)
{
	glm::mat4 finalTransformMatrix = Entity->GetTransform();
	for (IComponent* component : Entity->getLitComponents())
	{
		// transform components by entity transform
		finalTransformMatrix = finalTransformMatrix * ((MeshComponent*)component)->GetTransform();
		m_geometryPassShader.SetMat4("model", finalTransformMatrix);

		// check if mesh skinned
		m_geometryPassShader.SetUniform("isSkinned", ((MeshComponent*)component)->m_IsSkinned);
		if (((MeshComponent*)component)->m_IsSkinned)
			((MeshComponent*)component)->SetJointUniforms(m_geometryPassShader);

		// Draw Update Materials
		((MeshComponent*)component)->DrawUpdateMaterials(m_geometryPassShader);
	}
}

void GBuffer::drawLightingPass()
{
	// clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);
	// use the lighting pass shader
	m_lightingPassShader.use();
	// updating transforms
	UpdateTransformUniforms();
	// activating textures
	bindAllGBufferTextures();
	// set lighting uniforms
	setLightingUniforms();
}

void GBuffer::BlitDepthBuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_PostFXBuffer->GetFBO());// write to default HDR IFramebuffer
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
}

void GBuffer::Draw()
{
	// 0. calculate shadow textures
	calculateShadows();

	// 1. geometry pass: render scene's geometry/color data into gbuffer
	drawGeometryPass();

	// 1.5 SSAO Pass : draw SSAO in ViewSpace to be used during lighting pass
	((SSAOBuffer*)m_SSAOBuffer)->Draw(gViewPosition, gViewNormal);

	// 2. Render pass to PostFX buffer
	m_PostFXBuffer->Bind();

	// 3. lighting pass: calculate lighting using gbuffer textures
	drawLightingPass();
	RenderQuad();

	// 4. copy content of geometry's depth buffer to HDR buffer
	BlitDepthBuffer();

	// 5. Unbind postFX buffer
	m_PostFXBuffer->UnBind();
}

GBuffer::GBuffer(IFramebuffer*& PostFXBuffer)
{
	Initialize();
	mShadowbuffer = new ShadowBuffer();
	m_PostFXBuffer = PostFXBuffer;
}

GBuffer::~GBuffer()
{
}
