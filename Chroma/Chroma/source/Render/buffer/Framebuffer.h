#ifndef _CHROMA_FRAMEBUFFER_
#define _CHROMA_FRAMEBUFFER_
// stl
#include <iostream>
#include <vector>

// local
#include "screenmanager/ChromaScreenManagerConfig.h"
#include "shader/Shader.h"
#include "scene/ChromaScene.h"

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

	Shader* screenShader{new Shader(fragSource, vtxSource)};

	// quad data
	float quadData[20] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	// texture
	unsigned int FBOTexture;

	// setup
	virtual void initialize();
	void setupQuad();

	// Initializing
	virtual void GenTexture();
	virtual void SetTextureParameters();
	virtual void updateTransformUniforms();

	// draw
	void renderQuad();

public:

	// Getters/Setters
	void setTexture(unsigned int newFBOTexture);
	virtual unsigned int getTexture() { return FBOTexture; };

	virtual unsigned int getFBO() { return FBO; };
	void setResolutionScale(unsigned int newScale);

	void setScale(glm::vec2 newScale) { scale = newScale; };
	void setPosition(glm::vec2 newPosition) { offset = newPosition; };


	template<typename UniformType>
	void setUniform(std::string uniformName, UniformType uniformValue) {
		screenShader->setUniform(uniformName, uniformValue);
	};

	virtual void Bind();
	virtual void unBind();
	virtual void Draw();
	virtual void Draw(const bool& useBloom);
	virtual void Draw(unsigned int& gViewPosition, unsigned int& gNormal, const ChromaScene*& scene) {};

	Framebuffer();
	~Framebuffer();
};

#endif