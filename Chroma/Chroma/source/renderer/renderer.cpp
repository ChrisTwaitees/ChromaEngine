#include "Renderer.h"


void Renderer::updateShadowMappingUniforms(ChromaComponent* component)
{
	component->getShader()->use();
	component->getShader()->setMat4("lightSpaceMatrix", Shadowbuffer->getLightSpaceMatrix());
}

void Renderer::Init()
{
	Shadowbuffer = new ShadowBuffer(mScene);
}

void Renderer::RenderScene()
{
	// Shadowbuffer
	Shadowbuffer->calculateShadows();

	// ShadowBuffer Debug Framebuffer
	FrameBuffer.setTexture(Shadowbuffer->ShadowMapTexture.ShaderID);
	FrameBuffer.setScale(glm::vec2(0.25f));
	FrameBuffer.setPosition(glm::vec2(-0.5f));
	FrameBuffer.Draw();


	// HDR Tone Mapping

	HDRFrameBuffer->bind();
	// Render Scene
	for (ChromaEntity* entity : mScene->Entities)
	{
		for (ChromaComponent* component : entity->RenderableComponents)
		{
			updateShadowMappingUniforms(component);
		}
		entity->Draw(*mScene->RenderCamera, mScene->Lights);
	}

	// Draw HRD Tone Mapping
	HDRFrameBuffer->Draw();
}

Renderer::Renderer(const ChromaScene* Scene)
{
	mScene = Scene;
	Init();
}


Renderer::~Renderer()
{
}
