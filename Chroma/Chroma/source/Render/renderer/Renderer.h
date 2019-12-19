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
#include <buffer/PostFXBuffer.h>
#include <buffer/DebugBuffer.h>
#include <buffer/ForwardBuffer.h>
#include <model/SkyBox.h>
#include <shader/Shader.h>

class Renderer
{
private:
	// Start of Frame
	void clearBuffers();

	// DEFERRED LIGHTING RENDER
	void renderDefferedComponents();

	// FORWARD LIGHTING RENDER
	void renderForwardComponents();

	// DEBUG RENDER
	void renderDebug();

	// POST FX
	void renderPostFX();

protected:
	// SCENE
	ChromaScene* m_Scene;

	// SCREENMANAGER
	const ChromaScreenManager* m_screenManager;

	// Deffered Buffer
	Framebuffer* m_GBuffer;

	// Forward Buffer
	Framebuffer* m_forwardBuffer;

	// Debug Buffer
	DebugBuffer* m_debugBuffer;

	// Post FX
	Framebuffer* m_postFXBuffer{ new PostFXBuffer() };

	// INITIALIZE
	void Initialize();
public:

	void RenderScene();

	Renderer() {};
	Renderer(ChromaScene*& Scene, const ChromaScreenManager* ScreenManager);

	DebugBuffer* GetDebugBuffer() { return m_debugBuffer; };
	~Renderer();
};

#endif