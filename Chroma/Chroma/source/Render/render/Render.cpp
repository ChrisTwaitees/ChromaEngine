#include "Render.h"
#include <gui/GUI.h>

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

		((PostFXBuffer*)m_PostFXBuffer)->Draw(Chroma::GUI::m_Bloom);
	}

	void Render::RenderGraphicsDebug()
	{
		if (Chroma::GUI::m_DrawGraphicsDebug)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Albedo
			if (Chroma::GUI::m_GraphicsDebugSelected == 0)
			{
				m_GraphicsDebugBuffer->SetTexture(((GBuffer*)m_GBuffer)->GetAlbedoTexture());
				m_GraphicsDebugBuffer->Draw();
			}
			// Normals
			if (Chroma::GUI::m_GraphicsDebugSelected == 1)
			{
				m_GraphicsDebugBuffer->SetTexture(((GBuffer*)m_GBuffer)->GetNormalTexture());
				m_GraphicsDebugBuffer->Draw();
			}
			// MetRough
			if (Chroma::GUI::m_GraphicsDebugSelected == 2)
			{
				m_GraphicsDebugBuffer->SetTexture(((GBuffer*)m_GBuffer)->GetMetalRoughnessAO());
				m_GraphicsDebugBuffer->Draw();
			}
			// SSAO
			if (Chroma::GUI::m_GraphicsDebugSelected == 3)
			{
				m_GraphicsDebugBuffer->SetTexture(((GBuffer*)m_GBuffer)->GetSSAOTexture());
				m_GraphicsDebugBuffer->Draw();
			}
			// Shadows
			if (Chroma::GUI::m_GraphicsDebugSelected == 4)
			{
				m_GraphicsDebugBuffer->SetTexture(((GBuffer*)m_GBuffer)->GetShadowBufferTexture());
				m_GraphicsDebugBuffer->Draw();
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
		m_DebugBuffer = new DebugBuffer(m_PostFXBuffer);
		m_ForwardBuffer = new ForwardBuffer(m_PostFXBuffer);
		m_GraphicsDebugBuffer = new IFramebuffer();
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
		RenderGraphicsDebug();

		// Clear
		CleanUp();
	}

}


