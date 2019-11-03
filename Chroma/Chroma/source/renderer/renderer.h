#ifndef _CHROMA_RENDERER_
#define _CHROMA_RENDERER_

#include <memory>
#include <vector>

#include "../scene/ChromaScene.h"
#include "../shaders/Shader.h"
#include "../buffers/GBuffer.h"
#include "../buffers/Framebuffer.h"
#include "../buffers/ShadowBuffer.h"
#include "../buffers/PostFXBuffer.h"
#include "../models/SkyBox.h"
#include "../screenmanager/ChromaScreenManager.h"

class Renderer
{
private:
	// DEFERRED LIGHTING RENDER
	void renderDefferedComponents();

	// FORWARD RENDER
	void renderForwardComponents();
	void renderTransparency();

	// POST FX
	void renderPostFX();

protected:
	// SCENE
	const ChromaScene* mScene;

	// SCREENMANAGER
	const ChromaScreenManager* mScreenManager;

	// Skybox
	SkyBox* mSkybox;

	// Post FX
	Framebuffer* mPostFXBuffer{ new PostFXBuffer };

	// GBuffer
	Framebuffer* mGBuffer;

	// Debug Framebuffer
	Framebuffer debugFramebuffer;

	// INITIALIZE
	void Init();
public:

	void RenderScene();

	Renderer(const ChromaScene* Scene, const ChromaScreenManager* mScreenManager);
	~Renderer();
};

#endif