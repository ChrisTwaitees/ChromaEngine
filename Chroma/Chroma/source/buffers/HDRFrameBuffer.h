#ifndef _CHROMA_HDR_BUFFER_
#define _CHROMA_HDR_BUFFER_
#include "Framebuffer.h"

class HDRFramebuffer : public Framebuffer
{
protected:
	const char* vtxSource{ "resources/shaders/frameBufferVertex.glsl" };
	const char* fragSource{ "resources/shaders/fragHDRframebuffer.glsl" };


	float exposure{ 1.0f };

	void GenTexture() override;
	void initialize() override;

public:

	//Shader* getShader() override { return &screenShader; };

	void Draw() override;
	void bind() override;

	HDRFramebuffer();
	~HDRFramebuffer();
};


#endif