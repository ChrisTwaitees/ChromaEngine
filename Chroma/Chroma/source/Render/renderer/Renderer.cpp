#include "Renderer.h"


void Renderer::renderDefferedComponents()
{
	// GBUFFER
	m_GBuffer->Draw();
}

void Renderer::renderForwardComponents()
{
	// Render Forward Components
	// Enitities
	for (IChromaEntity*& entity : m_scene->getEntities())
	{
		// render unlit components
		if (entity->getUnlitComponents().size() > 0)
		{
			glm::mat4 finalTransformMatrix = entity->getTransformationMatrix();
			for (IChromaComponent*& component : entity->getUnlitComponents())
				((ChromaMeshComponent*)component)->DrawUpdateTransforms(*m_scene->getRenderCamera(), finalTransformMatrix);
		}
		// render transparent components
		if (m_scene->getTransparentEntities().size() > 0)
			renderTransparency();
	}
	// SkyBox
	if (m_screenManager->useSkybox)
		m_skybox->Draw();
}

void Renderer::renderTransparency()
{
	////glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Sorting for Transparency Shading
	std::map<float, IChromaEntity*> alpha_sorted;
	for (IChromaEntity* TransparentEntity : m_scene->getTransparentEntities())
	{
		float distance = glm::length(TransparentEntity->getPosition() - m_scene->getRenderCamera()->getPosition());
		alpha_sorted[distance] =  TransparentEntity;
	}
	// iterating from furthest to closest
	for (std::map<float, IChromaEntity*>::reverse_iterator it = alpha_sorted.rbegin(); it != alpha_sorted.rend(); ++it)
	{
		glm::mat4 finalTransformMatrix = it->second->getTransformationMatrix();
		for (IChromaComponent* component : it->second->getTransparentComponents())
			((ChromaMeshComponent*)component)->Draw(*m_scene->getRenderCamera(), m_scene->getLights(), finalTransformMatrix);
	}
}

void Renderer::renderDebug()
{
	if (m_screenManager->drawDebug)
		m_debugBuffer->Draw();
}

void Renderer::renderPostFX()
{
	m_screenManager->useBloom ? ((PostFXBuffer*)m_postFXBuffer)->Draw(true) : m_postFXBuffer->Draw();
	m_postFXBuffer->setUniform("exposure", m_screenManager->exposure);
	m_postFXBuffer->setUniform("gamma", m_screenManager->gamma);
}

void Renderer::Init()
{
	m_GBuffer = new GBuffer(m_scene, m_postFXBuffer);
	m_skybox = new SkyBox(m_scene->getRenderCamera());
	m_debugBuffer = new DebugBuffer(m_scene->getRenderCamera());
}

void Renderer::RenderScene()
{
	// Deferred
	renderDefferedComponents();

	// Forward
	renderForwardComponents();

	// Post FX
	renderPostFX();

	// Debug
	renderDebug();
}


Renderer::Renderer(ChromaScene*& Scene, const ChromaScreenManager* ScreenManager)
{
	m_scene = Scene;
	m_screenManager = ScreenManager;
	Init();
}

Renderer::~Renderer()
{
}
