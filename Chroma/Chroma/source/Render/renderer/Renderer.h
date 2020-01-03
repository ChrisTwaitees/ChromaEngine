#ifndef _CHROMA_RENDERER_
#define _CHROMA_RENDERER_

// stl
#include <memory>
#include <vector>

// chroma
#include <screen/Screen.h>
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
	void ClearBuffers();

	// DEFERRED LIGHTING RENDER
	void RenderDefferedComponents();

	// FORWARD LIGHTING RENDER
	void RenderForwardComponents();

	// DEBUG RENDER
	void RenderDebug();

	// POST FX
	void RenderPostFX();

protected:
	// SCENE
	Scene* m_Scene{ nullptr };

	// Deffered Buffer
	Framebuffer* m_GBuffer{ nullptr };

	// Forward Buffer
	Framebuffer* m_ForwardBuffer{ nullptr };

	// Debug Buffer
	DebugBuffer* m_DebugBuffer{ nullptr };

	// Post FX
	Framebuffer* m_PostFXBuffer{ new PostFXBuffer() };

	// INITIALIZE
public:

	void Init();
	void RenderScene();

	DebugBuffer* GetDebugBuffer() { return m_DebugBuffer; };

	Renderer() {};
	Renderer(Scene*& Scene);
	~Renderer();
};

#endif