#ifndef _CHROMA_FORWARD_BUFFER_
#define _CHROMA_FORWARD_BUFFER_
#include <buffer/Framebuffer.h>
#include <scene/Scene.h>

#include <ibl/IBL.h>

class ForwardBuffer : public Framebuffer
{

	// scene
	Scene* m_Scene{ nullptr };
	Framebuffer* m_PostFXBuffer{ nullptr };

	// functions
	void Initialize() override;
	void fetchColorAndDepth();
	void blitDepthBuffer();
	void renderForwardComponents();
	void renderTransparency();
	void AttachBuffer();
	void DrawQuad();

public:
	void Draw() override;

	ForwardBuffer(Scene* const& source_scene, Framebuffer* const& postFXBuffer);
	~ForwardBuffer();
};

#endif