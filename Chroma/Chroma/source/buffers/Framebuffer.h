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

	// position on screen
	glm::vec2 offset{ glm::vec2(0) };
	glm::vec2 scale{ glm::vec2(1) };

	Shader screenShader{fragSource, vtxSource};
	//Shader screenShader;

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
	void initialize();
	void setupQuad();

public:
	void setTexture(unsigned int newFBOTexture);
	unsigned int getTexture() { return FBOTexture; };
	void setResolutionScale(unsigned int newScale);

	void setScale(glm::vec2 newScale) { scale = newScale; };
	void setPosition(glm::vec2 newPosition) { offset = newPosition; };

	void bind();
	void Draw();

	Framebuffer();
	~Framebuffer();
};

#endif