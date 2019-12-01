#ifndef _CHROMA_FORWARD_BUFFER_
#define _CHROMA_FORWARD_BUFFER_
#include <buffer/Framebuffer.h>
#include <scene/ChromaScene.h>

#include <ibl/IBL.h>

class ForwardBuffer : public Framebuffer
{

	// scene
	ChromaScene* m_scene{ nullptr };
	Framebuffer* m_postFXBuffer{ nullptr };

	// functions
	void initialize() override;
	void fetchColorAndDepth();
	void blitDepthBuffer();
	void renderForwardComponents();
	void renderTransparency();
	void attachBuffer();
	void drawQuad();

public:
	void Draw() override;

	ForwardBuffer(ChromaScene* const& source_scene, Framebuffer* const& postFXBuffer);
	~ForwardBuffer();
};

#endif