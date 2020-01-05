#include "ForwardBuffer.h"

void ForwardBuffer::Initialize()
{
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	// create floating point color buffer
	glGenTextures(1, &m_FBOTexture);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBOTexture, 0);
	// create depth buffer (renderbuffer)
	glGenRenderbuffers(1, &m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	// attach buffers
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		CHROMA_WARN("Framebuffer not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}	

void ForwardBuffer::FetchColorAndDepth()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_PostFXBuffer->GetFBO()); // fetch  postFXBuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO); // copy depth and color to current buffer
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST
	);
}

void ForwardBuffer::BlitDepthBuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_PostFXBuffer->GetFBO());// write to default HDR IFramebuffer
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
}

void ForwardBuffer::RenderForwardComponents()
{
	// attach current buffer and copy contents of postfx buffer
	AttachBuffer();
	CopyDepthAndColor(m_PostFXBuffer->GetFBO(), m_FBO);

	// Render Skybox first for Transparent Entities
	Chroma::Scene::GetSkyBox()->Draw();

	// Render Unlit Components
	for (std::string const& UID : Chroma::Scene::GetEntityUIDs())
	{
		// render unlit components
		if (Chroma::Scene::GetEntity(UID)->getUnlitComponents().size() > 0)
		{
			glm::mat4 worldTransform = Chroma::Scene::GetEntity(UID)->GetTransform();
			for (IComponent*& component : Chroma::Scene::GetEntity(UID)->getUnlitComponents())
				((MeshComponent*)component)->DrawUpdateTransforms(*Chroma::Scene::GetRenderCamera(), worldTransform);
		}
	}

	// Render Transparent Entities
	if (Chroma::Scene::GetTransparentEntityUIDs().size() > 0)
		CHROMA_WARN("Transparency Not Implemented!");
		//RenderTransparency();
}

void ForwardBuffer::RenderTransparency()
{
	// set blend function before rendering any forward elements
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	// Disable Back Face Culling to allow interior of transparent objects to be seen
	glDisable(GL_CULL_FACE);
	// Sorting for Transparency Shading
	std::map<float, std::string> alpha_sorted;
	for (std::string const& UID : Chroma::Scene::GetTransparentEntityUIDs())
	{
		float distance = Chroma::Scene::GetEntityDistanceToCamera(UID);
		alpha_sorted[distance] = UID;
	}
	// iterating from furthest to closest
	for (std::map<float, std::string>::reverse_iterator it = alpha_sorted.rbegin(); it != alpha_sorted.rend(); ++it)
	{
		glm::mat4 worldTransform = Chroma::Scene::GetEntity(it->second)->GetTransform();
		for (IComponent* component : Chroma::Scene::GetEntity(it->second)->GetTransparentComponents())
		{
			if (((MeshComponent*)component)->m_IsForwardLit) // draw lit transparent components
				((MeshComponent*)component)->Draw(*Chroma::Scene::GetRenderCamera(), Chroma::Scene::GetLights(), worldTransform);
			else // draw unlit transparent components
				((MeshComponent*)component)->DrawUpdateTransforms(*Chroma::Scene::GetRenderCamera(), worldTransform);
		}
	}
	// Re enable backface culling for preventing unecessary rendering
	glEnable(GL_CULL_FACE);
}

void ForwardBuffer::AttachBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void ForwardBuffer::DrawQuad()
{
	// use screen shader
	m_ScreenShader->use();
	// using color attachment
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
	// setting transform uniforms
	UpdateTransformUniforms();
	RenderQuad();
}

void ForwardBuffer::Draw()
{
	// 1. Render Forward Components to m_FBO
	RenderForwardComponents();

	// 2. Copy Color and Depth from Forward Buffer to 
	// Post FX Buffer
	CopyDepthAndColor(m_FBO, m_PostFXBuffer->GetFBO());

	// 3. Set back top default buffer
	UnBind();
}

ForwardBuffer::ForwardBuffer(IFramebuffer* const& postFXBuffer)
{
	m_PostFXBuffer = postFXBuffer;

}

ForwardBuffer::~ForwardBuffer()
{
}