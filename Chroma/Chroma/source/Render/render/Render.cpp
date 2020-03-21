#include "Render.h"
#include <screen/Screen.h>
#include <Editor/ui/EditorUI.h>

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

	// Graphics Debug
	IFramebuffer* Render::m_GraphicsDebugBuffer;

	void Render::CleanUp()
	{
		// DEBUG BUFFER
		m_DebugBuffer->ClearColorAndDepth();
	}

	void Render::RenderDefferedComponents()
	{
		// DEFFERED BUFFER
		m_GBuffer->Draw();
	}

	void Render::RenderForwardComponents()
	{
		// FORWARD BUFFER
		m_ForwardBuffer->Draw();
	}

	void Render::RenderDebug()
	{
		// DEBUG BUFFER
		m_DebugBuffer->Draw();
	}

	void Render::RenderPostFX()
	{
		// POSTFX BUFFER
		m_PostFXBuffer->SetUniform("exposure", 1.0f);
		m_PostFXBuffer->SetUniform("gamma", 2.2f);

		static_cast<PostFXBuffer*>(m_PostFXBuffer)->Draw(Chroma::EditorUI::m_Bloom);
	}

	void Render::RenderGraphicsDebug()
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		switch (Chroma::UI::m_GraphicsDebugSelected)
		{
		case (0):
		{
			// Albedo
			m_GraphicsDebugBuffer->SetTexture(((GBuffer*)m_GBuffer)->GetAlbedoTexture());
			m_GraphicsDebugBuffer->Draw();
			break;
		}
		case (1):
		{
			// Normals
			m_GraphicsDebugBuffer->SetTexture(((GBuffer*)m_GBuffer)->GetNormalTexture());
			m_GraphicsDebugBuffer->Draw();
			break;
		}
		case (2):
		{
			// Met Rough AO 
			m_GraphicsDebugBuffer->SetTexture(((GBuffer*)m_GBuffer)->GetMetalRoughnessAO());
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
		case (4) :
		{
			// Shadow depth buffer
			m_GraphicsDebugBuffer->SetTexture(((GBuffer*)m_GBuffer)->GetShadowBufferTexture());
			m_GraphicsDebugBuffer->Draw();
			break;
		}
		}


	}

	void Render::Init()
	{
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

		// Buffers
		m_PostFXBuffer = new PostFXBuffer();
		m_GBuffer = new GBuffer(m_PostFXBuffer);
		m_ForwardBuffer = new ForwardBuffer(m_PostFXBuffer);
		m_DebugBuffer = new DebugBuffer(m_PostFXBuffer);
		m_GraphicsDebugBuffer = new IFramebuffer();

		// Set to Default dimensions
		Chroma::Screen::SetDimensions(SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	void Render::RenderScene()
	{
		// Deferred
		RenderDefferedComponents();

		// Forward
		RenderForwardComponents();

		// Debug
		RenderDebug();

		// Post FX
		RenderPostFX();

		// Graphics Debug
		if (Chroma::UI::m_DrawGraphicsDebug)
			RenderGraphicsDebug();

		// Clear
		CleanUp();
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
		Chroma::Scene::GetRenderCamera()->SetASPECT((float)width/(float)height);

		// update buffers
		m_PostFXBuffer->ScreenResizeCallback(width, height);
		m_GBuffer->ScreenResizeCallback(width, height);
		m_ForwardBuffer->ScreenResizeCallback(width, height);
		m_DebugBuffer->ScreenResizeCallback(width, height);
		m_GraphicsDebugBuffer->ScreenResizeCallback(width, height);

		// Draw while resizing
		RenderScene();
		Chroma::Screen::Update();
	}

	glm::mat4 Render::GetLightSpaceMatrix()
	{
		return ((GBuffer*)m_GBuffer)->GetLightSpaceMatrix();
	}

	void Render::BindShadowMaps()
	{
		((GBuffer*)m_GBuffer)->BindShadownMaps();
	}

}


