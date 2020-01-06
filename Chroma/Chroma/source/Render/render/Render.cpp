#include "Render.h"


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

		m_PostFXBuffer->Draw();
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

		// Clear
		CleanUp();
	}

}


