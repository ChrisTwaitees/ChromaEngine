#ifndef _CHROMA_RENDERER_
#define _CHROMA_RENDERER_

//common
#include <common/PrecompiledHeader.h>

// chroma
#include <scene/Scene.h>
#include <buffer/GBuffer.h>
#include <buffer/PostFXBuffer.h>
#include <buffer/DebugBuffer.h>
#include <buffer/ForwardBuffer.h>
#include <jobsystem/JobSystem.h>

namespace Chroma
{
	class Render
	{
	public:

		// INITIALIZE
		static void Init();
		static void RenderScene();

		// Screen resize
		static void ScreenResizeCallBack(int const& width, int const& height);

		// Getters
		static DebugBuffer*& GetDebugBuffer() { return m_DebugBuffer; };
		static glm::mat4 GetLightSpaceMatrix();

		// Functions
		static void BindShadowMaps();
	private:

		// Start of Frame
		static void CleanUp();

		// DEFERRED LIGHTING RENDER
		static void RenderDefferedComponents();

		// FORWARD LIGHTING RENDER
		static void RenderForwardComponents();

		// DEBUG RENDER
		static void RenderDebug();

		// POST FX
		static void RenderPostFX();

		// GRAPHICS DEBUG RENDER
		static void RenderGraphicsDebug();

		// Deffered Buffer
		static IFramebuffer* m_GBuffer;

		// Forward Buffer
		static IFramebuffer* m_ForwardBuffer;

		// Debug Buffer
		static DebugBuffer* m_DebugBuffer;

		// Post FX
		static IFramebuffer* m_PostFXBuffer;

		// Graphics Debug Buffer
		static IFramebuffer* m_GraphicsDebugBuffer;



	};
}



#endif