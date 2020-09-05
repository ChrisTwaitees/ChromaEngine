#include "Render.h"
#include <Editor/ui/EditorUI.h>
#include <ubo/UniformBufferCamera.h>
#include <ubo/UniformBufferLighting.h>
#include <event/CameraEvent.h>
#include <window/Window.h>
#include <buffer/GBuffer.h>
#include <buffer/PostFXBuffer.h>
#include <buffer/ForwardBuffer.h>
#include <shadow/ShadowBuffer.h>
#include <buffer/SSRBuffer.h>
#include <buffer/EditorViewportBuffer.h>
#include <buffer/VXGIBuffer.h>

namespace Chroma
{
	// Deffered Buffer
	IFramebuffer* Render::m_GBuffer;

	// Forward Buffer
	IFramebuffer* Render::m_ForwardBuffer;

	// Debug Buffer
	DebugBuffer* Render::m_DebugBuffer;

	// Post FX
	IFramebuffer* Render::m_PostFXBuffer;

	// SSR Buffer
	IFramebuffer* Render::m_SSRBuffer;

	// Shadow Buffer
	IFramebuffer* Render::m_ShadowBuffer;

	// Graphics Debug
	IFramebuffer* Render::m_GraphicsDebugBuffer;

	// Editor Viewport
	IFramebuffer* Render::m_EditorViewportBuffer;

	// VXGI
	IFramebuffer* Render::m_VXGIBuffer;

	// Uniform Buffer Objects
	UniformBuffer* Render::m_UBOCamera;
	UniformBuffer* Render::m_UBOLighting;
	std::vector<UniformBuffer*> Render::m_UniformBufferObjects;

	// Buffer Textures
	// - positions
	unsigned int Render::m_WSPositions;
	unsigned int Render::m_VSPositions;
	// - normals 
	unsigned int Render::m_WSNormals;
	unsigned int Render::m_VSNormals;
	// - lightSpace 
	unsigned int Render::m_FragPosLightSpace;
	// - albedo	
	unsigned int Render::m_Albedo;
	// - MetRoughAO
	unsigned int Render::m_MetRoughAO;
	// - Depth	 
	unsigned int Render::m_Depth;

	// API
	Render::API Render::s_API;

	void Render::CleanUp()
	{
		// DEBUG BUFFER
		m_DebugBuffer->ClearColorAndDepth();
	}

	void Render::RenderDefferedComponents()
	{
		CHROMA_PROFILE_FUNCTION();
		// DEFFERED BUFFER
		m_GBuffer->Draw();
	}

	void Render::RenderForwardComponents()
	{
		CHROMA_PROFILE_FUNCTION();
		// FORWARD BUFFER
		m_ForwardBuffer->Draw();
	}

	void Render::RenderDebug()
	{
		CHROMA_PROFILE_FUNCTION();
#if 1 // VXGI temp
		// Debug size of Voxel Cube
		std::pair<glm::vec3, glm::vec3> extents = static_cast<VXGIBuffer*>(m_VXGIBuffer)->GetVoxelGridHalfExtents();
		Render::GetDebugBuffer()->DrawOverlayBox(extents.first, extents.second, glm::vec3(1.0, 0.0, 0.0));
#endif


		// DEBUG BUFFER
		m_DebugBuffer->Draw();
	}

	void Render::RenderPostFX()
	{
		CHROMA_PROFILE_FUNCTION();
		// SSR
		m_SSRBuffer->Draw();

		// POSTFX BUFFER
		m_PostFXBuffer->SetUniform("exposure", 1.0f);
		m_PostFXBuffer->SetUniform("gamma", 2.2f);

#ifdef EDITOR

		if (EditorUI::m_VXGI)
		{
			// VXGI
			static_cast<VXGIBuffer*>(m_VXGIBuffer)->Draw(EditorUI::m_VXGIVisualization);
			if (EditorUI::m_VXGIVisualization)
				m_EditorViewportBuffer->CopyColor(m_VXGIBuffer->GetFBO(), m_EditorViewportBuffer->GetFBO());
			else
			{
				m_EditorViewportBuffer->Bind();
				// POSTFX 
				static_cast<PostFXBuffer*>(m_PostFXBuffer)->Draw(EditorUI::m_Bloom);
			}
		}
		else
		{
			m_EditorViewportBuffer->Bind();
			// POSTFX 
			static_cast<PostFXBuffer*>(m_PostFXBuffer)->Draw(EditorUI::m_Bloom);
		}
		m_EditorViewportBuffer->UnBind();
#else
		static_cast<PostFXBuffer*>(m_PostFXBuffer)->UnBind();
		static_cast<PostFXBuffer*>(m_PostFXBuffer)->Draw(true);
#endif

	}

	void Render::RenderGraphicsDebug()
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		switch (Chroma::UI::m_GraphicsDebugSelected)
		{
		case (0):
		{
			// Albedo
			m_GraphicsDebugBuffer->SetTexture(m_Albedo);
			m_GraphicsDebugBuffer->Draw();
			break;
		}
		case (1):
		{
			// Normals
			m_GraphicsDebugBuffer->SetTexture(m_WSNormals);
			m_GraphicsDebugBuffer->Draw();
			break;
		}
		case (2):
		{
			// Met Rough AO 
			m_GraphicsDebugBuffer->SetTexture(m_MetRoughAO);
			m_GraphicsDebugBuffer->Draw();
			break;
		}
		case (3):
		{
			// SSAO
			m_GraphicsDebugBuffer->SetTexture(((GBuffer*)m_GBuffer)->GetSSAOTexture());
			m_GraphicsDebugBuffer->Draw();
			break;
		}
		}
	}

	void Render::GenerateUniformBufferObjects()
	{
		// Camera View and Projection Matrices
		m_UBOCamera = new UniformBufferCamera();
		m_UniformBufferObjects.push_back(m_UBOCamera);

		// Lighting and Shadow Uniforms
		m_UBOLighting = new UniformBufferLighting();
		m_UniformBufferObjects.push_back(m_UBOLighting);
	}

	void Render::UpdateUniformBufferObjects()
	{
		m_UBOLighting->OnUpdate();
	}

	void Render::Init()
	{
		s_API = API::OpenGL;
		// Enabling Render Features
		// ---------------------------------------
		// Enable depth buffer
		glEnable(GL_DEPTH_TEST);
		// Enable Face Culling
		glEnable(GL_CULL_FACE);
		// Enabling MSAA
		glEnable(GL_MULTISAMPLE);
		// Setting Clear Color
		glClearColor(SCREEN_DEFAULT_COLOR.x, SCREEN_DEFAULT_COLOR.y, SCREEN_DEFAULT_COLOR.z, SCREEN_DEFAULT_COLOR.w);
		// Enable Blending
		glEnable(GL_BLEND);
		// enabled srgb framebuffers
		//glEnable(GL_FRAMEBUFFER_SRGB);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		// Uniform Buffer Objects
		GenerateUniformBufferObjects();

		// Buffer Textures
		GenerateBufferTextures();

		// Buffers
		m_PostFXBuffer = new PostFXBuffer();
		m_GBuffer = new GBuffer(m_PostFXBuffer);
		m_ForwardBuffer = new ForwardBuffer(m_PostFXBuffer);
		m_DebugBuffer = new DebugBuffer(m_PostFXBuffer);
		m_SSRBuffer = new SSRBuffer();
		m_GraphicsDebugBuffer = new IFramebuffer();
		m_ShadowBuffer = new ShadowBuffer();
		m_EditorViewportBuffer = new EditorViewportBuffer();
		m_VXGIBuffer = new VXGIBuffer();

		CHROMA_INFO("Renderer Initialized.");
	}

	void Render::RenderScene()
	{

		CHROMA_PROFILE_FUNCTION();

		// Update UBOs
		UpdateUniformBufferObjects();

		// Shadows
		static_cast<ShadowBuffer*>(m_ShadowBuffer)->DrawShadowMaps();

		// Deferred
		RenderDefferedComponents();

		// Forward
		RenderForwardComponents();

		// Debug
		RenderDebug();

		// Post FX
		RenderPostFX();
		

		// Clear
		CleanUp();
	}

	void Render::OnEvent(Event& e)
	{
		// dispatch 
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<CameraMovedEvent>(CHROMA_BIND_EVENT_STATIC_FN(Render::OnCameraMoved));
	}

	void Render::ResizeBuffers(int const& width, int const& height)
	{
		// debug
		CHROMA_INFO_UNDERLINE;
		CHROMA_WARN("RENDERER :: Screen Resized : {0} by {1}", width, height);
		CHROMA_INFO_UNDERLINE;

		// make sure the viewport matches the new window dimensions;  
		glViewport(0, 0, width, height);

		// update scene camera projection to window aspect ratio
		Chroma::Scene::GetRenderCamera()->SetAspectRatio((float)width/(float)height);
		m_UBOCamera->OnUpdate();

		// update buffers
		ResizeBufferTextures(width, height);
		m_PostFXBuffer->ScreenResizeCallback(width, height);
		m_GBuffer->ScreenResizeCallback(width, height);
		m_ForwardBuffer->ScreenResizeCallback(width, height);
		m_DebugBuffer->ScreenResizeCallback(width, height);
		m_GraphicsDebugBuffer->ScreenResizeCallback(width, height);
		m_SSRBuffer->ScreenResizeCallback(width, height);
		m_EditorViewportBuffer->ScreenResizeCallback(width, height);
		m_VXGIBuffer->ScreenResizeCallback(width, height);
	}

	glm::mat4 Render::GetLightSpaceMatrix()
	{
		return static_cast<ShadowBuffer*>(m_ShadowBuffer)->GetLightSpaceMatrix();
	}

	bool Render::OnCameraMoved(CameraMovedEvent& e)
	{
		m_UBOCamera->OnUpdate();

		static_cast<ShadowBuffer*>(m_ShadowBuffer)->CalculateCascadeLightSpaceMatrices();
		static_cast<ShadowBuffer*>(m_ShadowBuffer)->DrawShadowMaps();

		return true;
	}

	void Render::GenerateBufferTextures()
	{
		// - WS Positions
		glGenTextures(1, &m_WSPositions);
		glBindTexture(GL_TEXTURE_2D, m_WSPositions);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// - gViewPosition for SSAO viewspace position 
		glGenTextures(1, &m_VSPositions);
		glBindTexture(GL_TEXTURE_2D, m_VSPositions);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// - fragposLightSpace color buffer for shadowmapping
		glGenTextures(1, &m_FragPosLightSpace);
		glBindTexture(GL_TEXTURE_2D, m_FragPosLightSpace);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// SURFACE DATA
		// - albebo buffer
		glGenTextures(1, &m_Albedo);
		glBindTexture(GL_TEXTURE_2D, m_Albedo);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// - WS normal
		glGenTextures(1, &m_WSNormals);
		glBindTexture(GL_TEXTURE_2D, m_WSNormals);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// - VS Normal
		glGenTextures(1, &m_VSNormals);
		glBindTexture(GL_TEXTURE_2D, m_VSNormals);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// - metalness/rougness/ambient occlusion buffer
		glGenTextures(1, &m_MetRoughAO);
		glBindTexture(GL_TEXTURE_2D, m_MetRoughAO);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// - depth
		glGenTextures(1, &m_Depth);
		glBindTexture(GL_TEXTURE_2D, m_Depth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	}

	void Render::ResizeBufferTextures(const int& newWidth, const int& newHeight)
	{
		// gbuffer textures
		// - position color buffer
		glBindTexture(GL_TEXTURE_2D, m_WSPositions);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, newWidth, newHeight, 0, GL_RGB, GL_FLOAT, NULL);

		// - gViewPosition for SSAO viewspace position 
		glBindTexture(GL_TEXTURE_2D, m_VSPositions);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, newWidth, newHeight, 0, GL_RGB, GL_FLOAT, NULL);

		// - fragposLightSpace color buffer for shadowmapping
		glBindTexture(GL_TEXTURE_2D, m_FragPosLightSpace);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, newWidth, newHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		// - depth buffer
		glBindTexture(GL_TEXTURE_2D, m_Depth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, newWidth, newHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		// SURFACE DATA
		// - albebo buffer
		glBindTexture(GL_TEXTURE_2D, m_Albedo);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		// - normal buffer
		glBindTexture(GL_TEXTURE_2D, m_WSNormals);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, newWidth, newHeight, 0, GL_RGB, GL_FLOAT, NULL);

		// - gViewPosition for SSAO viewspace position 
		glBindTexture(GL_TEXTURE_2D, m_VSNormals);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_FLOAT, NULL);


		// - metalness/rougness/ambient occlusion buffer
		glBindTexture(GL_TEXTURE_2D, m_MetRoughAO);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	}

}


