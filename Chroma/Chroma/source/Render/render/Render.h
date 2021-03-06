#ifndef _CHROMA_RENDERER_
#define _CHROMA_RENDERER_

//common
#include <common/PrecompiledHeader.h>

// chroma
#include <scene/Scene.h>

//#include <jobsystem/JobSystem.h>
#include <buffer/DebugBuffer.h>
#include <ubo/UniformBuffer.h>

namespace Chroma
{
	class CameraMovedEvent;

	class Render
	{
	public:
		// INITIALIZE
		static void Init();
		static void RenderScene();
		static void OnUpdate();

		// events
		static void OnEvent(Event& e);

		// API
		enum class API
		{
			None = 0,
			OpenGL = 1
		};
		static API GetAPI() { return s_API; }
		static void SetAPI(API api) { s_API = api; }

		// Screen resize
		static void ResizeBuffers(int const& width, int const& height);

		// Getters
		static DebugBuffer*& GetDebugBuffer() { return m_DebugBuffer; };
		static IFramebuffer*& GetPostFXBuffer() { return m_PostFXBuffer; }
		static IFramebuffer*& GetForwardBuffer() { return m_ForwardBuffer; }
		static IFramebuffer*& GetGBuffer() { return m_GBuffer; }
		static IFramebuffer*& GetSSRBuffer() { return m_SSRBuffer; }
		static IFramebuffer*& GetShadowBuffer() { return m_ShadowBuffer; }
		static IFramebuffer*& GetEditorViewportBuffer() { return m_EditorViewportBuffer; }
		static IFramebuffer*& GetVXGIBuffer() { return m_VXGIBuffer; }

		static glm::mat4 GetLightSpaceMatrix();
		static inline std::vector<UniformBuffer*>& GetUniformBufferObjects() { return m_UniformBufferObjects; };

		// Buffers
		inline static unsigned int GetWSPositions() { return m_WSPositions; };
		inline static unsigned int GetVSPositions() { return m_VSPositions; };

		inline static unsigned int GetVSNormals() { return m_VSNormals; };
		inline static unsigned int GetWSNormals() { return m_WSNormals; };

		inline static unsigned int GetFragPosLightSpace() { return m_FragPosLightSpace; };

		inline static unsigned int GetAlbedo() { return m_Albedo; }

		inline static unsigned int GetMetRoughAO() { return m_MetRoughAO; }

		inline static unsigned int GetDepth() { return m_Depth; }

		inline static unsigned int GetDirectLightingShadows() { return m_DirectLightingShadows; }
		inline static unsigned int GetIndirectLighting() { return m_IndirectLighting; }

	private:
		// events
		static bool OnCameraMoved(CameraMovedEvent& e);

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
		// - Direct & Indirect Lighting
		static unsigned int m_DirectLightingShadows;
		static unsigned int m_IndirectLighting;


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

		// Deffered Buffer
		static IFramebuffer* m_GBuffer;

		// Forward Buffer
		static IFramebuffer* m_ForwardBuffer;

		// Debug Buffer
		static DebugBuffer* m_DebugBuffer;
		
		// SSR Buffer
		static IFramebuffer* m_SSRBuffer;

		// Shadow Buffer
		static IFramebuffer* m_ShadowBuffer;
		
		// Post FX
		static IFramebuffer* m_PostFXBuffer;

		// Graphics Debug Buffer
		static IFramebuffer* m_GraphicsDebugBuffer;

		// Editor Viewport Buffer
		static IFramebuffer* m_EditorViewportBuffer;

		// VXGI Buffer
		static IFramebuffer* m_VXGIBuffer;

		// Uniform Buffer Objects
		static void GenerateUniformBufferObjects();
		static void UpdateUniformBufferObjects();

		static std::vector<UniformBuffer*> m_UniformBufferObjects;
		static UniformBuffer* m_UBOCamera;
		static UniformBuffer* m_UBOLighting;

		// API
		static API s_API;

	};
}



#endif