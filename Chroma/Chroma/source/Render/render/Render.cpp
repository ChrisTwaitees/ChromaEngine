#include "Render.h"


namespace Chroma
{
	// Deffered Buffer
	Framebuffer* Render::m_GBuffer;

	// Forward Buffer
	Framebuffer* Render::m_ForwardBuffer;

	// Debug Buffer
	DebugBuffer* Render::m_DebugBuffer;

	// Post FX
	Framebuffer* Render::m_PostFXBuffer;

	void Render::ClearBuffers()
	{
		// DEBUG BUFFER
		m_DebugBuffer->ClearBuffer();
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
		m_PostFXBuffer->setUniform("exposure", 1.0f);
		m_PostFXBuffer->setUniform("gamma", 2.2f);

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
		ClearBuffers();
	}

}


