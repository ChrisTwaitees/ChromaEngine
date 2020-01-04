#ifndef _CHROMA_FORWARD_BUFFER_
#define _CHROMA_FORWARD_BUFFER_
#include <buffer/Framebuffer.h>
#include <scene/Scene.h>

#include <ibl/IBL.h>

class ForwardBuffer : public Framebuffer
{
	// scene
	Framebuffer* m_PostFXBuffer{ nullptr };

	// functions
	void Initialize() override;
	void fetchColorAndDepth();
	void blitDepthBuffer();
	void RenderForwardComponents();
	void renderTransparency();
	void AttachBuffer();
	void DrawQuad();

public:
	void Draw() override;

	ForwardBuffer(Framebuffer* const& postFXBuffer);
	~ForwardBuffer();
};

#endif