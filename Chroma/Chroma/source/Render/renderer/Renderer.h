#ifndef _CHROMA_RENDERER_
#define _CHROMA_RENDERER_

#include <memory>
#include <vector>

#include "scene/ChromaSceneManager.h"
#include "shader/Shader.h"
#include "buffer/GBuffer.h"
#include "buffer/Framebuffer.h"
#include "buffer/ShadowBuffer.h"
#include "buffer/PostFXBuffer.h"
#include "model/SkyBox.h"
#include "screenmanager/ChromaScreenManager.h"

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
	const ChromaSceneManager* mScene;

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

	Renderer(const ChromaSceneManager* Scene, const ChromaScreenManager* mScreenManager);
	~Renderer();
};

#endif