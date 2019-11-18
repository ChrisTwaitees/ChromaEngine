#include "DebugBuffer.h"

void DebugBuffer::drawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color)
{
	Bind();
	std::cout << "drawing line from debugBuffer" << std::endl;

	unBind();
}

void DebugBuffer::drawCircle(const glm::vec3& center, const float& radius, const glm::vec3& color)
{
}

void DebugBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DebugBuffer::Draw()
{
	unBind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screenShader->use();
	// using color attachment
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	// setting transform uniforms
	updateTransformUniforms();
	renderQuad();

	// clearing the FBO after rendering shapes
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	unBind();
}


