#include "Renderer.h"


void Renderer::renderDefferedComponents()
{
	// GBUFFER
	mGBuffer->Draw();
}



void Renderer::renderForwardComponents()
{
	// Render Forward Components
	for (ChromaEntity* entity : mScene->Entities)
	{
		// render unlit components
		if (entity->UnLitComponents.size() > 0)
		{
			glm::mat4 finalTransformMatrix = entity->getTransformationMatrix();
			for (IChromaComponent* component : entity->UnLitComponents)
				component->DrawUpdateTransforms(*mScene->RenderCamera, finalTransformMatrix);
		}
		// render transparent components
		if (entity->TransparentComponents.size() > 0)
			renderTransparencey(entity->TransparentComponents);
	}
}

void Renderer::updateShadowMappingUniforms(IChromaComponent* component)
{
	component->getShader()->use();
	component->getShader()->setMat4("lightSpaceMatrix", mShadowbuffer->getLightSpaceMatrix());
}

void Renderer::renderTransparencey(std::vector<IChromaComponent*> transparentComponents)
{
	////glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	mGBuffer = new GBuffer(mScene, mShadowbuffer, mHDRFrameBuffer);
	mSkybox = new SkyBox(*mScene->RenderCamera);
}

void Renderer::RenderScene()
{
	// Shadowbuffer
	mShadowbuffer->calculateShadows();

	// HDR Tone Mapping
	//mHDRFrameBuffer->Bind();

	////SkyBox
	//if (mScreenManager->useSkybox)
	//mSkybox->Draw();


	renderDefferedComponents();

	renderForwardComponents();



	// Draw HRD Tone Mapping
	//mHDRFrameBuffer->Draw(mScreenManager->useBloom);
	//mHDRFrameBuffer->setUniform("exposure", mScreenManager->exposure);
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
