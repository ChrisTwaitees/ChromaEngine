#ifndef _CHROMA_RENDERER_
#define _CHROMA_RENDERER_

#include "../scene/ChromaScene.h"
#include "../shaders/Shader.h"
#include "../buffers/Framebuffer.h"
#include "../buffers/ShadowBuffer.h"


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