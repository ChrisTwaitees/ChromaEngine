#ifndef _CHROMA_HDR_BUFFER_
#define _CHROMA_HDR_BUFFER_
#include "Framebuffer.h"

class PostFXBuffer : public Framebuffer
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
	void updateTransformUniforms() override;
	void configure_shaders();

public:

	unsigned int getFBO() override { return hdrFBO; };

	void Draw() override;
	void Draw(const bool& useBloom) override;
	void Bind() override;


	PostFXBuffer();
	~PostFXBuffer();
};


#endif