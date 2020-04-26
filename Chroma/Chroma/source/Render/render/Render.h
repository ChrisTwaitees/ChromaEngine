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
#include <buffer/SSRBuffer.h>
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
		static void ResizeBuffers(int const& width, int const& height);

		// Getters
		static DebugBuffer*& GetDebugBuffer() { return m_DebugBuffer; };
		static IFramebuffer*& GetPostFXBuffer() { return m_PostFXBuffer; }
		static IFramebuffer*& GetForwardBuffer() { return m_ForwardBuffer; }
		static IFramebuffer*& GetGBuffer() { return m_GBuffer; }
		static glm::mat4 GetLightSpaceMatrix();

		// Buffers
		inline static unsigned int GetWSPositions() { return m_WSPositions; };
		inline static unsigned int GetVSPositions() { return m_VSPositions; };

		inline static unsigned int GetVSNormals() { return m_VSNormals; };
		inline static unsigned int GetWSNormals() { return m_WSNormals; };

		inline static unsigned int GetFragPosLightSpace() { return m_FragPosLightSpace; };

		inline static unsigned int GetAlbedo() { return m_Albedo; }

		inline static unsigned int GetMetRoughAO() { return m_MetRoughAO; }

		inline static unsigned int GetDepth() { return m_Depth; }

		// Functions
		static void BindShadowMaps();
	private:

		// BUFFER TEXTURES
		static void GenerateBufferTextures();
		static void ResizeBufferTextures(const int& newWidth, const int& newHeight);
		// - positions
		static unsigned int m_WSPositions;
		static unsigned int m_VSPositions;
		// - normals
		static unsigned int m_WSNormals;
		static unsigned int m_VSNormals;
		// - lightSpace 
		static unsigned int m_FragPosLightSpace;
		// - albedo
		static unsigned int m_Albedo;
		// - MetRoughAO
		static unsigned int m_MetRoughAO;
		// - Depth
		static unsigned int m_Depth;

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
		
		// SSR Buffer
		static IFramebuffer* m_SSRBuffer;
		
		// Post FX
		static IFramebuffer* m_PostFXBuffer;

		// Graphics Debug Buffer
		static IFramebuffer* m_GraphicsDebugBuffer;

		



	};
}



#endif