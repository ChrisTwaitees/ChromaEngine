#ifndef _CHROMA_RENDERER_
#define _CHROMA_RENDERER_

// stl
#include <memory>
#include <vector>

// chroma
#include <scene/Scene.h>
#include <buffer/GBuffer.h>
#include <buffer/PostFXBuffer.h>
#include <buffer/DebugBuffer.h>
#include <buffer/ForwardBuffer.h>

namespace Chroma
{
	class Render
	{
		// Start of Frame
		static void ClearBuffers();

		// DEFERRED LIGHTING RENDER
		static void RenderDefferedComponents();

		// FORWARD LIGHTING RENDER
		static void RenderForwardComponents();

		// DEBUG RENDER
		static void RenderDebug();

		// POST FX
		static void RenderPostFX();

		// SCENE
		static Scene* m_Scene;

		// Deffered Buffer
		static Framebuffer* m_GBuffer;

		// Forward Buffer
		static Framebuffer* m_ForwardBuffer;

		// Debug Buffer
		static DebugBuffer* m_DebugBuffer;

		// Post FX
		static Framebuffer* m_PostFXBuffer;

		// INITIALIZE
	public:

		static void Init(Scene*& scene);
		static void RenderScene();

		static DebugBuffer*& GetDebugBuffer() { return m_DebugBuffer; };

	};
}



#endif