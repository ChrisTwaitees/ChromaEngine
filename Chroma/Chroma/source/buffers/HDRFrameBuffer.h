#ifndef _CHROMA_HDR_BUFFER_
#define _CHROMA_HDR_BUFFER_
#include "Framebuffer.h"

class HDRFramebuffer : public Framebuffer
{
protected:
	// default HDR shader
	unsigned int hdrFBO, hdrRBO;
	const char* vtxSource{ "resources/shaders/frameBufferVertex.glsl" };
	const char* fragSource{ "resources/shaders/fragBloomFrameBuffer.glsl" };
	// blur shader
	const char* blurfragSource{ "resources/shaders/fragBlur.glsl" };

	// Textures
	unsigned int colorBuffersTextures[2];

	// Bloom FBO and Textures
	void genBlurBuffer();
	unsigned int blurFBOs[2];
	unsigned int blurColorBuffers[2];

	// Blur
	Shader* blurShader;
	bool horizontal = true, first_iteration = true;
	int blurIterations{ 10 };
	void blurFragments();


	// HDR Attrs
	float exposure{ 1.0f };

	// Functions
	void initialize() override;
	void configure_shaders();

public:

	//Shader* getShader() override { return &screenShader; };

	void Draw() override;
	void Draw(bool useBloom) override;
	void bind() override;


	HDRFramebuffer();
	~HDRFramebuffer();
};


#endif