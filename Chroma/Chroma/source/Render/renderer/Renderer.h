#ifndef _CHROMA_RENDERER_
#define _CHROMA_RENDERER_

#include <memory>
#include <vector>

#include <scene/ChromaScene.h>
#include <entity/ChromaEntity.h>
#include <shader/Shader.h>
#include <buffer/GBuffer.h>
#include <buffer/Framebuffer.h>
#include <buffer/ShadowBuffer.h>
#include <buffer/PostFXBuffer.h>
#include <model/SkyBox.h>
#include <screenmanager/ChromaScreenManager.h>
#include <renderer/DebugRenderer.h>

class Renderer
{
private:
	// DEFERRED LIGHTING RENDER
	void renderDefferedComponents();

	// FORWARD RENDER
	void renderForwardComponents();
	void renderTransparency();

	// DEBUG RENDER
	void renderDebug();

	// POST FX
	void renderPostFX();

protected:
	// SCENE
	ChromaScene* m_scene;

	// SCREENMANAGER
	const ChromaScreenManager* m_screenManager;

	// Debug Renderer
	ChromaDebugRenderer* m_debugRenderer;

	// Skybox
	SkyBox* m_skybox;

	// Post FX
	Framebuffer* m_postFXBuffer{ new PostFXBuffer };

	// GBuffer
	Framebuffer* m_GBuffer;

	// INITIALIZE
	void Init();
public:

	void RenderScene();

	Renderer() {};
	Renderer(ChromaScene*& Scene, const ChromaScreenManager* ScreenManager, ChromaDebugRenderer*& DebugRenderer);
	~Renderer();
};

#endif