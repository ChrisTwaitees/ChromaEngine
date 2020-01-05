#ifndef _CHROMA_FORWARD_BUFFER_
#define _CHROMA_FORWARD_BUFFER_
#include <buffer/Framebuffer.h>
#include <scene/Scene.h>

#include <ibl/IBL.h>

class ForwardBuffer : public Framebuffer
{
	// scene
	Framebuffer* m_PostFXBuffer{ nullptr };

	unsigned int FBO_Transparent{ 0 };
	unsigned int FBO_Transparent_Depth{ 0 };
	// functions
	void Initialize() override;
	void FetchColorAndDepth();
	void BlitDepthBuffer();
	void RenderForwardComponents();
	void RenderTransparency();
	void AttachBuffer();
	void DrawQuad();

public:
	void Draw() override;

	ForwardBuffer(Framebuffer* const& postFXBuffer);
	~ForwardBuffer();
};

#endif