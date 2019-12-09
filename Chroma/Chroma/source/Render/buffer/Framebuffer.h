#ifndef _CHROMA_FRAMEBUFFER_
#define _CHROMA_FRAMEBUFFER_
// stl
#include <iostream>
#include <vector>

// Chroma
#include <screenmanager/ChromaScreenManagerConfig.h>
#include <shader/Shader.h>
#include <model/NDCPlanePrimitive.h>


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
	NDCPlanePrimitive quad;

	// position on screen
	glm::vec2 offset{ glm::vec2(0) };
	glm::vec2 scale{ glm::vec2(1) };

	Shader* screenShader{new Shader(fragSource, vtxSource)};

	// texture
	unsigned int FBOTexture;

	// setup
	virtual void Initialize();

	// Initializing
	virtual void GenTexture();
	virtual void SetTextureParameters();
	virtual void updateTransformUniforms();


public:

	// draw
	void renderQuad();
	// Getters/Setters
	void setTexture(unsigned int newFBOTexture);
	virtual unsigned int getTexture() { return FBOTexture; };

	virtual unsigned int getFBO() { return FBO; };

	void setScale(glm::vec2 newScale) { scale = newScale; };
	void setPosition(glm::vec2 newPosition) { offset = newPosition; };


	template<typename UniformType>
	void setUniform(std::string uniformName, UniformType uniformValue) {
		screenShader->setUniform(uniformName, uniformValue);
	};

	virtual void Bind();
	virtual void unBind();
	virtual void ClearBuffer();
	virtual void Draw();

	Framebuffer();
	virtual ~Framebuffer();
};

#endif