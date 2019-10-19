#ifndef _CHROMA_FRAMEBUFFER_
#define _CHROMA_FRAMEBUFFER_
// stl
#include <iostream>
#include <vector>

// local
#include "../screenmanager/ChromaScreenManagerConfig.h"
#include "../shaders/Shader.h"

class Framebuffer
{
protected:
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

	// position on screen
	glm::vec2 offset{ glm::vec2(0) };
	glm::vec2 scale{ glm::vec2(1) };

	Shader screenShader{fragSource, vtxSource};

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
	unsigned int FBOTexture;

	// setup
	virtual void initialize();
	void setupQuad();

	// Initializing
	virtual void GenTexture();
	virtual void SetTextureParameters();

	// draw
	void renderQuad();

public:
	// Getters/Setters
	void setTexture(unsigned int newFBOTexture);
	unsigned int getTexture() { return FBOTexture; };
	void setResolutionScale(unsigned int newScale);

	void setScale(glm::vec2 newScale) { scale = newScale; };
	void setPosition(glm::vec2 newPosition) { offset = newPosition; };



	virtual void bind();
	virtual void Draw();

	Framebuffer();
	~Framebuffer();
};

#endif