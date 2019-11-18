#ifndef _CHROMA_RENDERER_
#define _CHROMA_RENDERER_

// stl
#include <memory>
#include <vector>

// chroma
#include <screenmanager/ChromaScreenManager.h>
#include <scene/ChromaScene.h>
#include <entity/ChromaEntity.h>
#include <buffer/GBuffer.h>
#include <buffer/Framebuffer.h>
#include <buffer/ShadowBuffer.h>
#include <buffer/PostFXBuffer.h>
#include <buffer/DebugBuffer.h>
#include <model/SkyBox.h>
#include <shader/Shader.h>

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

	// GBuffer
	Framebuffer* m_GBuffer;

	// Skybox
	SkyBox* m_skybox;

	// Debug Renderer
	DebugBuffer* m_debugBuffer;

	// Post FX
	Framebuffer* m_postFXBuffer{ new PostFXBuffer() };


	// INITIALIZE
	void Init();
public:

	void RenderScene();

	Renderer() {};
	Renderer(ChromaScene*& Scene, const ChromaScreenManager* ScreenManager);

	DebugBuffer* getDebugBuffer() { return m_debugBuffer; };
	~Renderer();
};

#endif