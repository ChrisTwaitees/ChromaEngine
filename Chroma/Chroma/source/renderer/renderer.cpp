#include "Renderer.h"


void Renderer::updateShadowMappingUniforms(IChromaComponent* component)
{
	component->getShader()->use();
	component->getShader()->setMat4("lightSpaceMatrix", mShadowbuffer->getLightSpaceMatrix());
}

void Renderer::Init()
{
	mShadowbuffer = new ShadowBuffer(mScene);
	mGBuffer = new GBuffer(mScene, mShadowbuffer);
	mSkybox = new SkyBox(*mScene->RenderCamera);
}

void Renderer::RenderScene()
{
	// Shadowbuffer
	mShadowbuffer->calculateShadows();

	// ShadowBuffer Debug Framebuffer

	// HDR Tone Mapping
	mHDRFrameBuffer->Bind();

	// GBUFFER
	mGBuffer->Draw();

	debugFramebuffer.setTexture(mShadowbuffer->ShadowMapTexture.ID);
	debugFramebuffer.setScale(glm::vec2(0.25f));
	debugFramebuffer.setPosition(glm::vec2(-0.5f));
	debugFramebuffer.Draw();

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 //Render Scene
	//for (ChromaEntity* entity : mScene->Entities)
	//{
	//	for (IChromaComponent* component : entity->RenderableComponents)
	//	{
	//		updateShadowMappingUniforms(component);
	//	}
	//	entity->Draw(*mScene->RenderCamera, mScene->Lights);
	//}

	////SkyBox
	//if (mScreenManager->useSkybox)
	//	mSkybox->Draw();


	// Draw HRD Tone Mapping
	//mHDRFrameBuffer->Draw(mScreenManager->useBloom);
	mHDRFrameBuffer->setUniform("exposure", mScreenManager->exposure);
}

Renderer::Renderer(const ChromaScene* Scene, const ChromaScreenManager* ScreenManager)
{
	mScene = Scene;
	mScreenManager = ScreenManager;
	Init();
}


Renderer::~Renderer()
{
	delete mShadowbuffer;
	delete mGBuffer;
	delete mHDRFrameBuffer;
}
