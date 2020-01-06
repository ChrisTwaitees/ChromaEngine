#ifndef _CHROMA_FRAMEBUFFER_
#define _CHROMA_FRAMEBUFFER_
// stl
#include <iostream>
#include <vector>

// Chroma
#include <ChromaConfig.h>
#include <shader/Shader.h>
#include <model/NDCPlanePrimitive.h>
#include <core/Core.h>



class IFramebuffer
{
protected:
	// consts
	const char* vtxSource{ "resources/shaders/frameBufferVertex.glsl" };
	const char* fragSource{ "resources/shaders/frameBufferFrag.glsl" };

	unsigned int resolutionFactor{ 1 };

	// framebuffer / renderbuffer
	unsigned int m_FBO;
	unsigned int m_RBO;

	// vertex array / vertex buffer
	NDCPlanePrimitive quad;

	// position on screen
	glm::vec2 m_Offset{ glm::vec2(0) };
	glm::vec2 m_Scale{ glm::vec2(1) };

	Shader* m_ScreenShader{ new Shader(fragSource, vtxSource) };

	// texture
	unsigned int m_FBOTexture;

	// setup
	virtual void Initialize();

	// Initializing
	virtual void GenTexture();
	virtual void SetTextureParameters();
	virtual void UpdateTransformUniforms();


public:

	// static
	static void CopyColorAndDepth(unsigned int const& sourceFBO, unsigned int const& targetFBO);
	static void CopyDepth(unsigned int const& sourceFBO, unsigned int const& targetFBO);
	static void CopyColor(unsigned int const& sourceFBO, unsigned int const& targetFBO);
	// draw
	void RenderQuad();
	// Getters/Setters
	void SetTexture(unsigned int newFBOTexture);
	virtual unsigned int GetTexture() { return m_FBOTexture; };

	virtual unsigned int GetFBO() { return m_FBO; };

	void SetScale(glm::vec2 newScale) { m_Scale = newScale; };
	void SetPosition(glm::vec2 newPosition) { m_Offset = newPosition; };



	template<typename UniformType>
	void SetUniform(std::string uniformName, UniformType uniformValue) {
		m_ScreenShader->SetUniform(uniformName, uniformValue);
	};

	virtual void Bind();
	virtual void UnBind();
	virtual void ClearBuffer();
	virtual void Draw();

	IFramebuffer();
	virtual ~IFramebuffer();
};




#endif