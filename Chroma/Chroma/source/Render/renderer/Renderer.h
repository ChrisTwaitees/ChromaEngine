#ifndef _CHROMA_RENDERER_
#define _CHROMA_RENDERER_

// stl
#include <memory>
#include <vector>

// chroma
#include <screenmanager/ScreenManager.h>
#include <scene/Scene.h>
#include <entity/Entity.h>
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
	Scene* m_Scene;

	// SCREENMANAGER
	const ScreenManager* m_screenManager;

	// Deffered Buffer
	Framebuffer* m_GBuffer;

	// Forward Buffer
	Framebuffer* m_forwardBuffer;

	// Debug Buffer
	DebugBuffer* m_debugBuffer;

	// Post FX
	Framebuffer* m_PostFXBuffer{ new PostFXBuffer() };

	// INITIALIZE
	void Initialize();
public:

	void RenderScene();

	Renderer() {};
	Renderer(Scene*& Scene, const ScreenManager* ScreenManager);

	DebugBuffer* GetDebugBuffer() { return m_debugBuffer; };
	~Renderer();
};

#endif