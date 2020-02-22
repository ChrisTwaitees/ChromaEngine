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
public:
	// static
	void CopyColorAndDepth(unsigned int const& sourceFBO, unsigned int const& targetFBO);
	void CopyDepth(unsigned int const& sourceFBO, unsigned int const& targetFBO);
	void CopyColor(unsigned int const& sourceFBO, unsigned int const& targetFBO);

	// draw
	void RenderQuad();
	// Getters/Setters
	void SetTexture(unsigned int newFBOTexture);
	virtual unsigned int GetTexture() { return m_FBOTexture; };

	virtual unsigned int GetFBO() { return m_FBO; };

	void SetScale(glm::vec2 newScale) { m_Scale = newScale; };
	void SetTranslation(glm::vec2 newPosition) { m_Offset = newPosition; };


	template<typename UniformType>
	void SetUniform(std::string uniformName, UniformType uniformValue) {
		m_ScreenShader->SetUniform(uniformName, uniformValue);
	};

	virtual void Bind();
	virtual void UnBind();
	virtual void ClearColorAndDepth();
	virtual void Draw();
	virtual void ScreenResizeCallback(int const& width, int const& height);

	IFramebuffer();
	virtual ~IFramebuffer();

protected:
	// dimensions
	unsigned int m_Width{ SCREEN_WIDTH };
	unsigned int m_Height{ SCREEN_HEIGHT };

	// consts
	const char* vtxSource{ "resources/shaders/frameBufferVertex.glsl" };
	const char* fragSource{ "resources/shaders/frameBufferFrag.glsl" };

	// framebuffer / renderbuffer
	unsigned int m_FBO;
	unsigned int m_RBO;

	// vertex array / vertex buffer
	NDCPlanePrimitive m_Quad;

	// position on screen
	glm::vec2 m_Offset{ glm::vec2(0) };
	glm::vec2 m_Scale{ glm::vec2(1) };

	// shader
	Shader* m_ScreenShader{ new Shader(fragSource, vtxSource) };

	// texture
	unsigned int m_FBOTexture;

	// setup
	virtual void Initialize();

	// Initializing
	virtual void ResizeBuffers();
	virtual void GenTexture();
	virtual void SetTextureParameters();
	virtual void UpdateTransformUniforms();
};




#endif