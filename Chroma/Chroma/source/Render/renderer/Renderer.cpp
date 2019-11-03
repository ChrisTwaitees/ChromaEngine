#include "Renderer.h"


void Renderer::renderDefferedComponents()
{
	// GBUFFER
	mGBuffer->Draw();
}

void Renderer::renderForwardComponents()
{
	// Render Forward Components
	// Enitities
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
		if (mScene->TransparentEntities.size() > 0)
			renderTransparency();
	}
	// SkyBox
	if (mScreenManager->useSkybox)
		mSkybox->Draw();
}

void Renderer::renderTransparency()
{
	////glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Sorting for Transparency Shading
	std::map<float, ChromaEntity*> alpha_sorted;
	for (ChromaEntity* TransparentEntity : mScene->TransparentEntities)
	{
		float distance = glm::length(TransparentEntity->getPosition() - mScene->RenderCamera->get_position());
		alpha_sorted[distance] =  TransparentEntity;
	}
	// iterating from furthest to closest
	for (std::map<float, ChromaEntity*>::reverse_iterator it = alpha_sorted.rbegin(); it != alpha_sorted.rend(); ++it)
	{
		glm::mat4 finalTransformMatrix = it->second->getTransformationMatrix();
		for (IChromaComponent* component : it->second->TransparentComponents)
			component->Draw(*mScene->RenderCamera, mScene->Lights, finalTransformMatrix);
	}
}

void Renderer::renderPostFX()
{
	mScreenManager->useBloom ? mPostFXBuffer->Draw(true) : mPostFXBuffer->Draw();
	mPostFXBuffer->setUniform("exposure", mScreenManager->exposure);
	mPostFXBuffer->setUniform("gamma", mScreenManager->gamma);
}

void Renderer::Init()
{
	mGBuffer = new GBuffer(mScene, mPostFXBuffer);
	mSkybox = new SkyBox(*mScene->RenderCamera);
}

void Renderer::RenderScene()
{
	// 

	// Deferred
	renderDefferedComponents();

	// Forward
	renderForwardComponents();

	// Post FX
	renderPostFX();
}

Renderer::Renderer(const ChromaSceneManager* Scene, const ChromaScreenManager* ScreenManager)
{
	mScene = Scene;
	mScreenManager = ScreenManager;
	Init();
}

Renderer::~Renderer()
{
}
