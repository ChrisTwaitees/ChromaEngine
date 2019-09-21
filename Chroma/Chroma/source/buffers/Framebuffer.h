#ifndef _CHROMA_FRAMEBUFFER_
#define _CHROMA_FRAMEBUFFER_
// stl
#include <iostream>
#include <vector>
// glad
#include <glad/glad.h>
// local
#include "../screenmanager/ChromaScreenManagerConfig.h"
#include "../shaders/Shader.h"

class Framebuffer
{
private:
	// consts
	const char* vtxSource{ "resources/shaders/frameBufferVertex.glsl" };
	const char* fragSource{ "resources/shaders/frameBufferFrag.glsl" };

	unsigned int resolutionFactor{1};

	// framebuffer / renderbuffer
	unsigned int FBO;
	unsigned int RBO;

	// vertex array / vertex buffer
	unsigned int VAO;
	unsigned int VBO;

	//const Shader& screenShader{Shader(fragSource, vtxSource)};
	Shader screenShader;

	// quad data
	std::vector<float> quadData = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// texture
	unsigned int texColorBuffer;

	// setup
	void initialize();
	void setupQuad();

public:
	unsigned int get_texColor() { return texColorBuffer; };
	void setResolutionScale(unsigned int newScale);

	void bind();
	void draw();

	Framebuffer();
	~Framebuffer();
};

#endif