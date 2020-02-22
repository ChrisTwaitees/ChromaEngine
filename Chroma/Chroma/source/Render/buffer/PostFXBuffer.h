#ifndef _CHROMA_HDR_BUFFER_
#define _CHROMA_HDR_BUFFER_

#include <buffer/IFramebuffer.h>

class PostFXBuffer : public IFramebuffer
{
protected:
	// default HDR shader
	unsigned int hdrFBO;
	const char* vtxSource{ "resources/shaders/frameBufferVertex.glsl" };
	const char* fragSource{ "resources/shaders/fragBloomFrameBuffer.glsl" };
	// blur shader
	const char* blurfragSource{ "resources/shaders/fragBlur.glsl" };

	// Textures
	unsigned int colorBuffersTextures[2];

	// Bloom m_FBO and Textures
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
	void Initialize() override;
	void UpdateTransformUniforms() override;
	void ConfigureShaders();
	void ResizeBuffers() override;

public:

	unsigned int GetFBO() override { return hdrFBO; };

	void Draw() override;
	void Draw(const bool& useBloom);
	void Bind() override;


	PostFXBuffer();
	~PostFXBuffer();
};


#endif