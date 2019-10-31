#ifndef _CHROMA_RENDERER_
#define _CHROMA_RENDERER_

#include <memory>
#include <vector>
#include "../scene/ChromaScene.h"
#include "../shaders/Shader.h"
#include "../buffers/Framebuffer.h"
#include "../buffers/ShadowBuffer.h"
#include "../buffers/HDRFrameBuffer.h"
#include "../buffers/GBuffer.h"
#include "../models/SkyBox.h"
#include "../screenmanager/ChromaScreenManager.h"

class Renderer
{
private:
	// LIGHTING
	void updateShadowMappingUniforms(IChromaComponent* component);

protected:
	// SCENE
	const ChromaScene* mScene;

	// SCREENMANAGER
	const ChromaScreenManager* mScreenManager;

	// Skybox
	SkyBox* mSkybox;
	
	// Shadow Buffer
	ShadowBuffer* mShadowbuffer;

	// GBuffer
	Framebuffer* mGBuffer;

	// HDR FrameBuffer // Post FX
	Framebuffer* mHDRFrameBuffer{ new HDRFramebuffer };

	// Debug Framebuffer
	Framebuffer debugFramebuffer;

	// Forward Render Alpha
	void renderTransparencey(std::vector<IChromaComponent*> ForwardComponents);


	// INITIALIZE
	void Init();
public:

	void RenderScene();

	Renderer(const ChromaScene* Scene, const ChromaScreenManager* mScreenManager);
	~Renderer();
};

#endif