#ifndef _CHROMA_RENDERER_
#define _CHROMA_RENDERER_

#include <memory>
#include "../scene/ChromaScene.h"
#include "../shaders/Shader.h"
#include "../buffers/Framebuffer.h"
#include "../buffers/ShadowBuffer.h"
#include "../buffers/HDRFrameBuffer.h"


class Renderer
{
private:
	// LIGHTING
	void updateShadowMappingUniforms(ChromaComponent* component);

protected:
	// SCENE
	const ChromaScene* mScene;

	// SHADOW MAPPING
	ShadowBuffer* Shadowbuffer;

	// HDR FrameBuffer
	Framebuffer* HDRFrameBuffer{ new HDRFramebuffer };

	// FRAME BUFFER
	Framebuffer FrameBuffer;



	// INITIALIZE
	void Init();
public:

	void RenderScene();
	Renderer(const ChromaScene* Scene);
	~Renderer();
};

#endif