#include "ForwardBuffer.h"

void ForwardBuffer::Initialize()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// create floating point color buffer
	glGenTextures(1, &FBOTexture);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOTexture, 0);
	// create depth buffer (renderbuffer)
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	// attach buffers
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}	

void ForwardBuffer::fetchColorAndDepth()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_postFXBuffer->getFBO()); // fetch  postFXBuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO); // copy depth and color to current buffer
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST
	);
}

void ForwardBuffer::blitDepthBuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_postFXBuffer->getFBO());// write to default HDR Framebuffer
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
}

void ForwardBuffer::renderForwardComponents()
{
	// attach current buffer and copy contents of postfx buffer
	attachBuffer();
	fetchColorAndDepth();

	// Render Skybox first for Transparent Entities
	m_Scene->GetSkyBox()->Draw();

	// Render Unlit Components
	for (IEntity* const& entity : m_Scene->GetEntities())
	{
		// render unlit components
		if (entity->getUnlitComponents().size() > 0)
		{
			glm::mat4 worldTransform = entity->GetTransformationMatrix();
			for (IComponent*& component : entity->getUnlitComponents())
				((MeshComponent*)component)->DrawUpdateTransforms(*m_Scene->GetRenderCamera(), worldTransform);
		}
	}

	// Render Transparent Entities
	if (m_Scene->GetTransparentEntities().size() > 0)
		renderTransparency();
}

void ForwardBuffer::renderTransparency()
{
	// Enable Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Sorting for Transparency Shading
	std::map<float, IEntity*> alpha_sorted;
	for (IEntity* TransparentEntity : m_Scene->GetTransparentEntities())
	{
		float distance = glm::length(TransparentEntity->GetPosition() - m_Scene->GetRenderCamera()->GetPosition());
		alpha_sorted[distance] = TransparentEntity;
	}
	// iterating from furthest to closest
	for (std::map<float, IEntity*>::reverse_iterator it = alpha_sorted.rbegin(); it != alpha_sorted.rend(); ++it)
	{
		glm::mat4 worldTransform = it->second->GetTransformationMatrix();
		for (IComponent* component : it->second->getTransparentComponents())
		{
			if (((MeshComponent*)component)->m_IsForwardLit) // draw lit transparent components
				((MeshComponent*)component)->Draw(*m_Scene->GetRenderCamera(), m_Scene->GetLights(), worldTransform);
			else // draw unlit transparent components
				((MeshComponent*)component)->DrawUpdateTransforms(*m_Scene->GetRenderCamera(), worldTransform);
		}
	}
	glDisable(GL_BLEND);
}

void ForwardBuffer::attachBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void ForwardBuffer::drawQuad()
{
	// use screen shader
	screenShader->use();
	// using color attachment
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	// setting transform uniforms
	updateTransformUniforms();
	renderQuad();
}

void ForwardBuffer::Draw()
{
	// 1. Render Forward Components to FBO
	renderForwardComponents();

	// 2. Bind postFX buffer to draw to
	m_postFXBuffer->Bind();

	// 3. Draw Quad
	drawQuad();

	// 4. copy content of depth buffer to Post FX Buffer
	blitDepthBuffer();

	// 5. Unbind postFX buffer
	m_postFXBuffer->unBind();
}

ForwardBuffer::ForwardBuffer(Scene* const& source_scene, Framebuffer* const& postFXBuffer)
{
	m_Scene = source_scene;
	m_postFXBuffer = postFXBuffer;

}

ForwardBuffer::~ForwardBuffer()
{
}