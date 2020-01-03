#include "Renderer.h"


void Renderer::ClearBuffers()
{
	// DEBUG BUFFER
	m_DebugBuffer->ClearBuffer();
}

void Renderer::RenderDefferedComponents()
{
	// DEFFERED BUFFER
	m_GBuffer->Draw();
}

void Renderer::RenderForwardComponents()
{
	// FORWARD BUFFER
	m_ForwardBuffer->Draw();
}

void Renderer::RenderDebug()
{
	// DEBUG BUFFER
	m_DebugBuffer->Draw();	
}

void Renderer::RenderPostFX()
{
	// POSTFX BUFFER
	m_PostFXBuffer->setUniform("exposure", 1.0);
	m_PostFXBuffer->setUniform("gamma", 2.2);
}

void Renderer::Init()
{
	// OpenGL 3
// glad: load all OpenGL function pointers
// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		CHROMA_FATAL("Failed to Initialize GLAD");
	}

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

	// Buffers
	m_GBuffer = new GBuffer(m_Scene, m_PostFXBuffer);
	m_DebugBuffer = new DebugBuffer(m_Scene->GetRenderCamera(), m_PostFXBuffer);
	m_ForwardBuffer = new ForwardBuffer(m_Scene, m_PostFXBuffer);
}

void Renderer::RenderScene()
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


Renderer::Renderer(Scene*& Scene)
{
	m_Scene = Scene;
	Initialize();
}

Renderer::~Renderer()
{
	delete m_Scene;
	delete m_GBuffer;
	delete m_DebugBuffer;
	delete m_PostFXBuffer;
}
