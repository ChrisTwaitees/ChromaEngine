#include "Renderer.h"


void Renderer::updateShadowMappingUniforms(IChromaComponent* component)
{
	component->getShader()->use();
	component->getShader()->setMat4("lightSpaceMatrix", mShadowbuffer->getLightSpaceMatrix());
}

void Renderer::renderTransparencey(std::vector<IChromaComponent*> transparentComponents)
{
	// Sorting for Transparency Shading
	 //Sorting Grass for Transparencey Shading
	std::map<float, IChromaComponent*> sorted;
	for (unsigned int i = 0; i < transparentComponents.size(); i++)
	{
		float distance = glm::length(transparentComponents[i]->getPosition() - mScene->RenderCamera->get_position());
		sorted[distance] = transparentComponents[i];
	}
	// iterating from furthest to closest
	for (std::map<float, IChromaComponent*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		glm::mat4 finalTransformMatrix = it->second->getTransformationMatrix();
		it->second->Draw(*mScene->RenderCamera, mScene->Lights, finalTransformMatrix);
	}
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
	//mHDRFrameBuffer->Bind();

	////SkyBox
	//if (mScreenManager->useSkybox)
	//	mSkybox->Draw();

	// GBUFFER
	mGBuffer->Draw();



	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 // FORWARD RENDER
	for (ChromaEntity* entity : mScene->Entities)
	{
		for (IChromaComponent* component : entity->ForwardComponents)
		{
		//	updateShadowMappingUniforms(component);
		}
		entity->Draw(*mScene->RenderCamera, mScene->Lights);
	}



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
