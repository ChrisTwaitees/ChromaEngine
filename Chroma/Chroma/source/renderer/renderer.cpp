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

	FrameBuffer.setTexture(Shadowbuffer->ShadowMapTexture.ShaderID);
	FrameBuffer.setScale(glm::vec2(0.25f));
	FrameBuffer.setPosition(glm::vec2(-0.5f));
	FrameBuffer.Draw();

	for (ChromaEntity* entity : mScene->Entities)
	{
		for (ChromaComponent* component : entity->RenderableComponents)
		{
			updateShadowMappingUniforms(component);
		}
		entity->Draw(*mScene->RenderCamera, mScene->Lights);
	}
}

Renderer::Renderer(const ChromaScene* Scene)
{
	mScene = Scene;
	Init();
}


Renderer::~Renderer()
{
}
